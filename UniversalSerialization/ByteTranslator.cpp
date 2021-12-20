#include <cstddef>

module ByteTranslator;

namespace Smore::Serialization {
	template <typename T>
	std::size_t Mark(const T* object, SerializationContextBase& buffer) {
		return 0;
	}
}
