#include <span>

export module SerializationContextBase;

export namespace Smore::Serialization {

	struct SerializationContextBase {
		virtual ~SerializationContextBase() = default;
		virtual std::size_t push(std::span<const std::byte> span) = 0;
	};
	
	template <typename T>
	struct SerializationTypeContextBase;
}