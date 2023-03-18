#include "Core/FormatString.hpp"

namespace kurs
{
	std::string formatString(std::string_view format, va_list args)
	{
		constexpr std::size_t c_NiceCaseBufferSize = 256;
		std::array<char, c_NiceCaseBufferSize> niceCaseBuffer = {};
		std::unique_ptr<char[]> worstCaseBuffer;
		char* utilizedBuffer = niceCaseBuffer.data();
	
		va_list args_copy;
		va_copy(args_copy, args);

		size_t requiredSize = vsnprintf(nullptr, 0, format.data(), args_copy) + 1;
		if (requiredSize > c_NiceCaseBufferSize)
		{
			worstCaseBuffer = std::make_unique<char[]>(requiredSize);
			utilizedBuffer = worstCaseBuffer.get();
		}

		vsnprintf(utilizedBuffer, requiredSize, format.data(), args_copy);

		va_end(args_copy);

		return std::string(utilizedBuffer, requiredSize);
	}
}
