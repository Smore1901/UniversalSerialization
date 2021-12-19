#pragma once
#include <iterator>
#include <span>


namespace Smore::Serialization {

	template <typename>
	struct ByteTranslator;

	template <typename T>
	concept CompileTimeKnown = !std::is_polymorphic_v<T> || std::is_final_v<T>;

	template <typename T>
	concept AbstractClass = std::is_abstract_v<T>;

	template <typename T>
	concept PolymorphicClass = std::is_polymorphic_v<T>;

	struct SerializationContextBase {
		virtual ~SerializationContextBase() = default;
		virtual std::size_t push(std::span<const std::byte> span) = 0;
	};

	template <typename T>
	struct SerializationTypeContextBase;

	template <CompileTimeKnown T>
	struct SerializationTypeContextBase<T> : virtual SerializationContextBase {
		std::map<size_t, std::array<std::byte, ByteTranslator<T>::Size>> _cache;
		std::set<T*> _toDo;
		std::mutex _lock;

		template <std::input_iterator I>
		std::size_t Mark(I begin, I end) {
			std::lock_guard g{ _lock };
			std::for_each(begin, end, [&](auto& p) {
				if (!_cache.contains(p)) {
					_toDo.emplace(p);
				}
				});
		}
	};

	template <AbstractClass T>
	struct SerializationTypeContextBase<T> : virtual SerializationContextBase {

		virtual std::vector<std::size_t> Dispatch(std::vector<const T*> pointers) = 0;

		template <std::input_iterator I>
		std::vector<std::size_t> Mark(I begin, I end) {
			std::vector<std::pair<std::type_index, T*>> cache;
			std::for_each(begin, end, [&](auto* ptr) {
				cache.emplace_back(typeid(ptr), ptr);
				});
			Dispatch
		}
	};

	template <PolymorphicClass T>
	struct SerializationTypeContextBase<T> : virtual SerializationContextBase {
		std::map<size_t, std::array<std::byte, ByteTranslator<T>::Size>> _cache;
		std::set<T*> _toDo;
		std::mutex _lock;

		template <InputIterator I>
		std::vector<std::size_t> Mark(I begin, I end) {
			std::lock_guard<g> _lock;
			std::for_each(begin, end, [&](auto& p) {
				if (!_cache.contains(p)) {
					_toDo.emplace(p);
				}
				});
		}
	};
}