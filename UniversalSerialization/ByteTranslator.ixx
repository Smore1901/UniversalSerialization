import SerializationContextBase;

#include <array>
#include <memory>
#include <numeric>
#include <optional>
#include <span>
#include <string>		
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

#include "boost/mp11/list.hpp"
#include "boost/mp11/algorithm.hpp"
#include "boost/range/irange.hpp"

export module ByteTranslator;

namespace {

	template <size_t FieldCount, typename T>
	constexpr auto UnsafeAsTuple(T&& pod) {
		if constexpr (FieldCount == 0) {
			return std::tuple<>{};
		}
		else if constexpr (FieldCount == 1) {
			auto& [a0] = pod;
			return std::forward_as_tuple(a0);
		}
		else if constexpr (FieldCount == 2) {
			auto& [a0, a1] = pod;
			return std::forward_as_tuple(a0, a1);
		}
		else if constexpr (FieldCount == 3) {
			auto& [a0, a1, a2] = pod;
			return std::forward_as_tuple(a0, a1, a2);
		}
		else if constexpr (FieldCount == 4) {
			auto& [a0, a1, a2, a3] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3);
		}
		else if constexpr (FieldCount == 5) {
			auto& [a0, a1, a2, a3, a4] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4);
		}
		else if constexpr (FieldCount == 6) {
			auto& [a0, a1, a2, a3, a4, a5] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5);
		}
		else if constexpr (FieldCount == 7) {
			auto& [a0, a1, a2, a3, a4, a5, a6] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6);
		}
		else if constexpr (FieldCount == 8) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7);
		}
		else if constexpr (FieldCount == 9) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8);
		}
		else if constexpr (FieldCount == 10) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
		}
		else if constexpr (FieldCount == 11) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
		}
		else if constexpr (FieldCount == 12) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
		}
		else if constexpr (FieldCount == 13) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
		}
		else if constexpr (FieldCount == 14) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
		}
		else if constexpr (FieldCount == 15) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
		}
		else if constexpr (FieldCount == 16) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
		}
		else if constexpr (FieldCount == 17) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
		}
		else if constexpr (FieldCount == 18) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
		}
		else if constexpr (FieldCount == 19) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
		}
		else if constexpr (FieldCount == 20) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
		}
		else if constexpr (FieldCount == 21) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
		}
		else if constexpr (FieldCount == 22) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
		}
		else if constexpr (FieldCount == 23) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
		}
		else if constexpr (FieldCount == 24) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23);
		}
		else if constexpr (FieldCount == 25) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24);
		}
		else if constexpr (FieldCount == 26) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25);
		}
		else if constexpr (FieldCount == 27) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
		}
		else if constexpr (FieldCount == 28) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27);
		}
		else if constexpr (FieldCount == 29) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28);
		}
		else if constexpr (FieldCount == 30) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29);
		}
		else if constexpr (FieldCount == 31) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
		}
		else if constexpr (FieldCount == 32) {
			auto& [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31] = pod;
			return std::forward_as_tuple(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
		}
		else {
			return;
		}
	}


	template<typename T, typename...TArgs>
	auto is_aggregate_constructable_impl(std::tuple<TArgs...>) -> decltype(T{ std::declval<TArgs>()... });

	template<typename T, typename TArgs, typename = void>
	struct is_aggregate_constructable : std::false_type {};

	template<typename T, typename TArgs>
	struct is_aggregate_constructable<T, TArgs, std::void_t<decltype(is_aggregate_constructable_impl<T>(std::declval<TArgs>()))>> : std::true_type {};

	// Checks if type can be initialized from braced-init-list.
	template <typename T, typename TArgs>
	constexpr auto is_aggregate_constructable_v = is_aggregate_constructable<T, TArgs>::value;

	// Class is convertible to anything.
	class any {
	public:
		template<typename T>
		operator T() const;
	};

	template <class T, typename...TArgs>
	constexpr std::size_t num_bindings_impl() {
		if constexpr (is_aggregate_constructable<T, std::tuple<TArgs...>>()) {
			return num_bindings_impl<T, any, TArgs...>();
		}
		else {
			return sizeof...(TArgs) - 1;
		}
	};

	template<typename T>
	constexpr auto struct_bind_num_v = num_bindings_impl<T, any>();

	struct _simplePredicates {
		template <typename T>
		struct is_pointer_like : std::false_type {};

		template <typename T>
		struct is_pointer_like<T*> : std::true_type {
			using type = T;
		};

		template <typename T>
		struct is_pointer_like<std::shared_ptr<T>> : std::true_type {
			using type = T;
		};
		template <typename T>
		struct is_pointer_like<std::unique_ptr<T>> : std::true_type {
			using type = T;
		};
		template <typename T>
		struct is_pointer_like<std::weak_ptr<T>> : std::true_type {
			using type = T;
		};
	};
}

using namespace boost::mp11;

export namespace Smore::Serialization {
	template <typename T>
	concept Fundamental = std::is_fundamental_v<T>;

	template<typename T>
	concept Aggregate = std::is_aggregate_v<T>;

	template <typename T>
	constexpr bool IsPointerLike = _simplePredicates::is_pointer_like<T>::value;

	template <typename T>
	using IsPointerLikeT = _simplePredicates::is_pointer_like<T>;

	template <typename T>
	concept PointerLike = _simplePredicates::is_pointer_like<T>::value;

	template <typename T>
	using Dereferenced = typename _simplePredicates::is_pointer_like<T>::type;

	template<typename T>
	struct Predicates {
		template <typename U>
		struct IsBaseOf : std::is_base_of<U, T> {};
		template <typename U>
		struct IsDerivedFrom : std::is_base_of<T, U> {};

	};

	struct SerializationContextBase;

	template<typename T>
	struct ByteTranslator;

	template <Fundamental T>
	struct ByteTranslator<T> {
		static constexpr size_t Size = sizeof(T);

		using PointerTypes = mp_list<>;

		static void Serialize(std::span<std::byte> span, const T value, SerializationContextBase&)
		{
			std::memcpy(span.data(), &value, Size);
		}
	};

	template <typename T, size_t N>
	struct ByteTranslator<std::array<T, N>> {

		static constexpr size_t ItemSize = Serialize<T>::Size;
		static constexpr size_t Size = ItemSize * N;

		using PointerTypes = ByteTranslator<T>::PointerTypes;

		static void Serialize(std::span<std::byte> span, const std::array<T, N>& value, SerializationContextBase& buffer) {

			[&] <size_t ... Indices>(std::index_sequence<Indices>) {
				((Serialize<T>::Serialize(span.subspan(Indices * ItemSize, ItemSize), value[Indices], buffer)), ...);
			}(std::make_index_sequence<N>{});
		}
	};

	template <typename ... Ts>
	struct ByteTranslator<std::variant<Ts...>> {
		static constexpr size_t Size = sizeof(size_t) + std::max(Serialize<Ts>::Size);

		using PointerTypes = mp_unique<mp_append<typename ByteTranslator<Ts>::PointerTypes...>>;

		static void Serialize(std::span<std::byte> span, const std::variant<Ts...>& value, SerializationContextBase& buffer) {
			Serialize<size_t>::Serialize(span.subspan(0, Serialize<size_t>::Size), value.index(), buffer);

			// TODO Implement
		}
	};

	template <typename ... Ts>
	struct ByteTranslator<std::tuple<Ts...>> {
		static constexpr size_t Size = [] {
			if constexpr (sizeof...(Ts) == 0) {
				return 0;
			}
			else {
				return ((ByteTranslator<Ts>::Size) + ...);
			}
		}();

		using PointerTypes = mp_unique<mp_append<typename ByteTranslator<Ts>::PointerTypes...>>;

		static void Serialize(std::span<std::byte> span, const std::variant<Ts...>& value, SerializationContextBase& buffer) {
			constexpr std::array sizes{ ByteTranslator<Ts>::Size ... };
			constexpr auto offsets = [] {
				std::array<size_t, sizeof...(Ts)> ret;
				std::exclusive_scan(sizes.begin(), sizes.end(), ret, 0);
				return ret;
			}();

			[&] <size_t ... Indices> (std::index_sequence<Indices...>) {
				ByteTranslator<mp_at_c<mp_list<Ts...>, Indices>>::Serialize(span.subspan(offsets[Indices], sizes[Indices]), std::get<Indices>(value), buffer);
			}(std::make_index_sequence<sizeof...(Ts)>{});
		}
	};

	template <typename T>
	struct ByteTranslator<std::vector<T>> {
		static constexpr size_t ItemSize = Serialize<T>::Size;
		static constexpr size_t Size = 2 * sizeof(size_t);

		using PointerTypes = ByteTranslator<T>::PointerTypes;

		static void Serialize(std::span<std::byte> span, const std::vector<T>& value, SerializationContextBase& buffer) {
			ByteTranslator<size_t>::Serialize(span.subspan(0, Serialize<size_t>::Size), value.size());
			if (value.size() == 0) {
				return;
			}


			std::vector<std::byte> tmp;
			tmp.resize(value.size() * ItemSize);
			auto range = boost::irange<size_t>(0, value.size());
			for (auto index : range) {
				ByteTranslator<T>::Serialize(std::span<std::byte>{tmp.data() + index * ItemSize, ItemSize}, value[index], buffer);
			}
			auto offset = buffer.push(tmp);
			ByteTranslator<size_t>::Serialize(span.subspan(Serialize<size_t>::Size, Serialize<size_t>::Size), offset);
		}
	};

	template <typename T>
	struct ByteTranslator<std::basic_string<T>> {
		static constexpr size_t ItemSize = Serialize<T>::Size;
		static constexpr size_t Size = 2 * sizeof(size_t);

		using PointerTypes = mp_list<>;

		static void Serialize(std::span<std::byte> span, const std::basic_string<T>& value, SerializationContextBase& buffer) {
			ByteTranslator<size_t>::Serialize(span.subspan(0, Serialize<size_t>::Size), value.size());
			if (value.size() == 0) {
				return;
			}

			std::vector<std::byte> tmp;
			tmp.resize(value.size() * ItemSize);
			auto range = boost::irange<size_t>(0, value.size());
			for (auto index : range) {
				ByteTranslator<T>::Serialize(std::span<std::byte>{tmp.data() + index * ItemSize, ItemSize}, value[index], buffer);
			}

			auto offset = buffer.push(tmp);
			ByteTranslator<size_t>::Serialize(span.subspan(Serialize<size_t>::Size, Serialize<size_t>::Size), offset, buffer);
		}
	};

	template <typename T>
	struct ByteTranslator<std::optional<T>> {
		static constexpr size_t Size = ByteTranslator<bool>::Size + ByteTranslator<T>::Size;

		using PointerTypes = ByteTranslator<T>::PointerTypes;

		static void Serialize(std::span<std::byte> span, const std::optional<T>& value, SerializationContextBase& buffer) {
			ByteTranslator<bool>::Serialize(span.subspan(0, ByteTranslator<bool>::Size), value.has_value());

			if (value.has_value()) {
				ByteTranslator<T>::Serialize(span.subspan(ByteTranslator<bool>::Size, ByteTranslator<T>::Size), *value, buffer);
			}
		}
	};

	template <typename T>
	size_t Mark(const T* object, SerializationContextBase& buffer);

	template <PointerLike T>
	struct ByteTranslator<T> {
		static constexpr size_t Size = sizeof(size_t);

		using ObjectType = std::decay_t<decltype(*T)>;
		using PointerTypes = mp_list<>;

		static void Serialize(std::span<std::byte> span, const T& value, SerializationContextBase& buffer)
		{
			ByteTranslator<bool>::Serialize(span.subspan(0, ByteTranslator<bool>::Size), Mark<ObjectType>(value, buffer));
		}
	};

	template <Aggregate T>
	struct ByteTranslator<T> {
		static constexpr size_t ElementCount = num_bindings_impl<T, any>();

		static constexpr auto AsRefTuple(T& pod) {
			return UnsafeAsTuple<ElementCount>(pod);
		}
		static constexpr auto AsRefTuple(const T& pod) {
			return UnsafeAsTuple<ElementCount>(pod);
		}

		using RefTupleT = decltype(AsRefTuple(std::declval<T&>()));
		using TupleT = mp_transform<std::remove_reference_t, RefTupleT>;

		template <size_t Index>
		using Ts = boost::mp11::mp_at_c<TupleT, Index>;

		static void Serialize(std::span<std::byte> span, const T& value, SerializationContextBase& buffer) {
			auto tuple = AsRefTuple(value);
			[&] <size_t ... Indices> (std::index_sequence<Indices...>) {
				constexpr std::array sizes{ ByteTranslator<Ts>::Size ... };
				constexpr auto offsets = [] {
					std::array<size_t, sizeof...(Ts)> ret;
					std::exclusive_scan(sizes.begin(), sizes.end(), ret, 0);
					return ret;
				}();

				((ByteTranslator<mp_at_c<TupleT, Indices>>::Serialize(span.subspan(offsets[Indices], sizes[Indices]), std::get<Indices>(tuple), buffer)), ...);
			}(std::make_index_sequence<ElementCount>{});
		}
	};
}