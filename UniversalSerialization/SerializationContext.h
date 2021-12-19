#pragma once

#include <iterator>
#include <map>
#include <mutex>
#include <set>
#include <vector>

#include "SerializationContextBase.h"

namespace Smore::Serialization {


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