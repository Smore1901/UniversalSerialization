#include "SerializationContext.h"

struct I1 {
	virtual ~I1() = default;
};
struct I2 {
	virtual ~I2() = default;
};
struct B1 {
	struct DataV1 {
		int a;
		int* b;
		std::shared_ptr<I1> c;
		std::shared_ptr<I1> d;
	};
	struct DataV2 {
		int* b;
		std::shared_ptr<I2> c;
	};
	using SerializationVersions = boost::mp11::mp_list<DataV1, DataV2>;
};
struct B2 : I1 {
	struct DataV1 {
		int* b;
	};
	using SerializationVersions = boost::mp11::mp_list<DataV1>;
};
struct B3 : I2 {
	struct DataV1 { };
	struct DataV2 { };
	struct DataV3 { };

	using SerializationVersions = boost::mp11::mp_list<DataV1, DataV2, DataV3>;
};
struct B4 : I1, I2 {
	struct DataV1 { };
	using SerializationVersions = boost::mp11::mp_list<DataV1>;
};

/*
struct SerializerCommon {
protected:

	struct SerializationContext {
		std::vector<std::byte> _buffer;
		std::mutex _lock;

		size_t push(std::span<const std::byte> data) {
			std::lock_guard g{ _lock };
			auto offset = _buffer.size();
			_buffer.resize(offset + data.size());
			std::copy(data.begin(), data.end(), _buffer.data() + offset);
			return offset;
		};

		template <typename T>
		size_t Mark(const T*);
	};

	struct TypeEntry {
		size_t Count;
		size_t Version;
	};

	class any {
	public:
		template<typename T>
		operator T() const;
	};


public:


	template <typename T>
	struct TypeVersionInterface {
	public:
		static constexpr size_t ElementCount = num_bindings_impl<T, any>();

		static constexpr auto AsRefTuple(T& pod) {
			return UnsafeAsTuple<ElementCount>(pod);
		}
		static constexpr auto AsRefTuple(const T& pod) {
			return UnsafeAsTuple<ElementCount>(pod);
		}

		using RefTupleT = decltype(AsRefTuple(std::declval<T&>()));
		using TupleT = boost::mp11::mp_transform<std::remove_reference_t, RefTupleT>;
		using PointerTypes = boost::mp11::mp_unique<boost::mp11::mp_transform<Dereferenced, boost::mp11::mp_filter<IsPointerLikeT, TupleT>>>;
		using PolymorphicTypes = boost::mp11::mp_filter<std::is_polymorphic, PointerTypes>;

		static constexpr size_t Size = ByteTranslator<TupleT>::Size;
	};

	template <typename T>
	struct TypeInterface {
		using SerializationVersions = typename T::SerializationVersions;

		static constexpr size_t VersionCount = boost::mp11::mp_size<SerializationVersions>::value;

		template <size_t VersionNumber>
		using Version = boost::mp11::mp_at_c<SerializationVersions, VersionNumber>;
		template <size_t VersionNumber>
		using VersionInterface = TypeVersionInterface<Version<VersionNumber>>;

		template <typename>
		struct _pointerTypesImpl;
		template <size_t ... Indices>
		struct _pointerTypesImpl<std::index_sequence<Indices...>> {
			using PointerTypes = boost::mp11::mp_unique<typename boost::mp11::mp_append<typename VersionInterface<Indices>::PointerTypes...>>;
			using PolymorphicTypes = boost::mp11::mp_unique<typename boost::mp11::mp_append<typename VersionInterface<Indices>::PolymorphicTypes...>>;
		};

		using PointerTypes = typename _pointerTypesImpl<std::make_index_sequence<VersionCount>>::PointerTypes;
		using PolymorphicTypes = typename _pointerTypesImpl<std::make_index_sequence<VersionCount>>::PolymorphicTypes;
	};
};

template <typename ... Types>
struct Serializer : SerializerCommon {
	static constexpr size_t SerializationTypeCount = sizeof...(Types);
	using SerializationTypes = boost::mp11::mp_list<Types...>;
	template <size_t Index>
	using SerializationType = boost::mp11::mp_at_c<SerializationTypes, Index>;

	using PointerTypes = boost::mp11::mp_unique<typename boost::mp11::mp_append<typename TypeInterface<Types>::PointerTypes...>>;
	using PolymorphicTypes = boost::mp11::mp_unique<typename boost::mp11::mp_append<typename TypeInterface<Types>::PolymorphicTypes...>>;

	template <typename T>
	using DerivedFrom = boost::mp11::mp_copy_if<PolymorphicTypes, ::Predicates<T>::template IsDerivedFrom>;

	template <typename T>
	using BaseOf = boost::mp11::mp_copy_if<SerializationTypes, ::Predicates<T>::template IsBaseOf>;

	template <typename T>
	struct Meta {
		enum class ID : size_t {};
	};

	template <typename T>
	struct ISerializer {
		virtual std::shared_ptr<T> GetSharedObject(typename Meta<T>::ID) = 0;
		virtual std::weak_ptr<T> GetWeakObject(typename Meta<T>::ID) = 0;
		virtual std::unique_ptr<T> GetUniqueObject(typename Meta<T>::ID) && = 0;
		virtual T* GetPlainObject(typename Meta<T>::ID) const = 0;
	};

	template <typename T, typename I>
	struct SerializationConverter : ISerializer<T> {
		std::shared_ptr<I> GetSharedObject(typename Meta<T>::ID obj, std::in_place_type_t<T>)
		{
			return ISerializer<T>::GetSharedObject(obj);
		}
		std::weak_ptr<I> GetWeakObject(typename Meta<T>::ID obj, std::in_place_type_t<T>)
		{
			return ISerializer<T>::GetWeakObject(obj);
		}
		std::unique_ptr<I> GetUniqueObject(typename Meta<T>::ID obj, std::in_place_type_t<T>)&&
		{
			return ISerializer<T>::GetUniqueObject(obj);
		}
		I* GetPlainObject(typename Meta<T>::ID obj, std::in_place_type_t<T>)
		{
			return ISerializer<T>::GetPlainObject(obj);
		}
	};

	template <typename T, typename U = DerivedFrom<T>>
	struct TypeSerializer;


	template <typename T, typename ... Is>
	struct TypeSerializer<T, std::tuple<Is...>> : SerializationConverter<T, Is>..., ISerializer<T> {

		static constexpr auto EntrySize = []<typename ... Versions>(boost::mp11::mp_list<Versions...>) {
			return std::array{ sizeof(Versions) + sizeof(size_t) ... };
		}(typename T::SerializationVersions{});

		static constexpr auto SerializationVersionCount = boost::mp11::mp_size<typename T::SerializationVersions>::value;

		std::vector<std::pair<typename Meta<T>::ID, std::variant<std::unique_ptr<T>, std::shared_ptr<T>, T*>>> _objects;

		void Initialize(std::span<std::byte> data, TypeEntry type) {
			_objects.resize(type.Count);
			auto range = boost::irange<size_t>(0, type.Count);
			std::for_each(std::execution::par_unseq, range.begin(), range.end(), [&](size_t index) {
				auto& kv = _objects[index];
				auto* ptr = (kv.second.emplace<0>(std::make_unique<T>())).get();
				});
		}

		std::shared_ptr<T> GetSharedObject(typename Meta<T>::ID id) final {
			auto& p = get(id);
			EnsureShared(p);
			return std::get<1>(p);
		}

		std::weak_ptr<T> GetWeakObject(typename Meta<T>::ID id) final {
			auto& p = get(id);
			EnsureShared(p);
			return std::get<1>(p);
		}

		std::unique_ptr<T> GetUniqueObject(typename Meta<T>::ID id) && final {
			auto& p = get(id);
			assert(p.index() == 0);
			auto unique = std::move(std::get<0>(p));
			p.emplace<2>(unique.get());
			return std::move(unique);
		}

		T* GetPlainObject(typename Meta<T>::ID id) const final {
			auto& p = get(id);
			switch (p.index()) {
			case 0:
				return std::get<0>(p).get();
			case 1:
				return std::get<1>(p).get();
			case 2:
				return std::get<2>(p);
			default:
				assert(false);
				std::abort();
			}
		}
	private:
		auto& get(typename Meta<T>::ID obj) {
			auto iter = std::lower_bound(_objects.begin(), _objects.end(), obj, [&](auto& pair, auto index) {
				return static_cast<size_t>(pair.first) < static_cast<size_t>(index);
				});
			assert(iter->first == obj);
			return iter->second;
		}
		auto& get(typename Meta<T>::ID obj) const {
			auto iter = std::lower_bound(_objects.begin(), _objects.end(), obj, [&](auto& pair, auto index) {
				return static_cast<size_t>(pair.first) < static_cast<size_t>(index);
				});
			assert(iter->first == obj);
			return iter->second;
		}
		void EnsureShared(auto& p) {
			switch (p.index()) {
			case 0:
			{
				auto unique = std::move(std::get<0>(p));
				p.emplace<1>(std::move(unique));
				break;
			}
			case 1:
				break;
			default:
				assert(false);
			}
		}
	};

	using SerializerSet = boost::mp11::mp_apply<std::tuple, boost::mp11::mp_transform<TypeSerializer, SerializationTypes>>;

	SerializerSet serializers_{};

	template <typename T, bool Unique>
	auto Materialize(std::span<std::byte> data) {

		std::array<TypeEntry, SerializationTypeCount> entryCounts;
		std::memcpy(entryCounts.data(), data.data(), sizeof(entryCounts));
		auto ranges = GetTableRanges(data, entryCounts);
		[&] <size_t ... Indices>(std::index_sequence<Indices...>) {
			((std::get<Indices>(serializers_).Initialize(entryCounts[Indices], ranges[Indices])), ...);
		}(std::make_index_sequence<SerializationTypeCount>{});
	}

	template <size_t ... Indices>
	auto GetTableRanges(std::span<std::byte> data, const std::array<TypeEntry, SerializationTypeCount>& entryCounts, std::index_sequence<Indices...> = std::make_index_sequence<SerializationTypeCount>{}) {

		std::array sizes{ entryCounts[Indices].Count * SerializationType<Indices>::EntrySizes[entryCounts[Indices]] ... };
		std::array<size_t, SerializationTypeCount> offsets;
		std::exclusive_scan(sizes.begin(), sizes.end(), offsets.begin(), sizeof(entryCounts));
		return std::array{ data.subspan(offsets[Indices], sizes[Indices])... };
	}
};

template <typename T>
void fail() {
	static_assert(false);
}

*/
int main() {
	using S = Smore::Serialization::SerializationContext<B1, B2, B3, B4>;
	// S s;

	// static_assert(std::is_same_v<S::PointerTypes, std::tuple<int, I1, I2>>);
	// static_assert(std::is_same_v<S::PolymorphicTypes, std::tuple<I1, I2>>);
	//fail<SerializerCommon::TypeVersionInterface<B1::DataV1>::TupleT>();
	//fail<SerializerCommon::TypeVersionInterface<B1::DataV2>::PolymorphicTypes>();
}