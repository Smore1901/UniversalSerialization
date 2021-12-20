#pragma once

import ByteTranslator;
import SerializationContextBase;

#include <iterator>
#include <map>
#include <mutex>
#include <set>
#include <span>
#include <typeindex>
#include <vector>
#include <xutility>

export module SerializationContext;

namespace {
	template <typename T>
	concept CompileTimeKnown = !std::is_polymorphic_v<T> || std::is_final_v<T>;

	template <typename T>
	concept AbstractClass = std::is_abstract_v<T>;

	template <typename T>
	concept PolymorphicClass = std::is_polymorphic_v<T>;
}

export namespace Smore::Serialization {

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
			// Dispatch
		}
	};

	template <PolymorphicClass T>
	struct SerializationTypeContextBase<T> : virtual SerializationContextBase {
		std::map<size_t, std::array<std::byte, ByteTranslator<T>::Size>> _cache;
		std::set<T*> _toDo;
		std::mutex _lock;

		template <std::input_iterator I>
		std::vector<std::size_t> Mark(I begin, I end) {
			std::lock_guard g{ _lock };
			std::for_each(begin, end, [&](auto& p) {
				if (!_cache.contains(p)) {
					_toDo.emplace(p);
				}
				});
		}
	};

	template <typename T, typename ... Ts>
	struct SerializationTypeContextDispatch;

	template <AbstractClass T, typename ... Ts>
	struct SerializationTypeContextDispatch<T, Ts ...> : SerializationTypeContextBase<T> {

		static const auto& GetCasters() {
			static const std::array casters{ {std::type_index{typeid(Ts)},+[](const T* ptr) {return static_cast<void*>(dynamic_cast<Ts>(ptr)); }}... };
		}
		virtual std::vector<std::size_t> Dispatch(std::vector<const T*> pointers) = 0;

	};

	template <typename ... Ts>
	struct SerializationContext : virtual SerializationContextBase, SerializationTypeContextDispatch<Ts, Ts...>... {

	};
}