#pragma once

#include "kurspch.h"

namespace kurs
{
	std::string formatString(std::string_view format, va_list args);

	inline std::string formatString(std::string_view format, ...)
	{
		va_list args;
		va_start(args, format);
		std::string formattedStr = formatString(format, args);
		va_end(args);

		return formattedStr;
	}
}
