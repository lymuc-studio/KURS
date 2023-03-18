#pragma once

#include <iostream>

#include "kurspch.hpp"

#include "Core/FuncName.hpp"
#include "Core/CompilerInfo.hpp"

namespace kurs::detail
{
	template<typename T>
	constexpr std::string_view getTypeNameGnu()
	{
		constexpr std::string_view funcName = KURS_FUNCNAME;

		constexpr std::size_t typeNameStart = funcName.find_first_of("=") + 2;
		constexpr std::size_t typeNameEnd = funcName.find_first_of(";");

		return funcName.substr(typeNameStart, typeNameEnd - typeNameStart);
	}

	template<typename T>
	constexpr std::string_view getTypeNameClang()
	{
		constexpr std::string_view funcName = KURS_FUNCNAME;

		constexpr std::size_t typeNameStart = funcName.find_first_of("=") + 2;
		constexpr std::size_t typeNameEnd = funcName.find_last_of("]");

		return funcName.substr(typeNameStart, typeNameEnd - typeNameStart);
	}

	template<typename T>
	constexpr std::string_view getTypeNameMsvc()
	{
		constexpr std::string_view funcSig = KURS_FUNCNAME;
		constexpr std::string_view funcName = __func__;

		constexpr std::string_view templateSigStart = funcSig.substr(
			funcSig.find(funcName)
		);

		constexpr std::size_t typeNameStart = templateSigStart.find_first_of("<") + 1;
		constexpr std::size_t typeNameEnd = templateSigStart.find_last_of(">");

		return templateSigStart.substr(typeNameStart, typeNameEnd - typeNameStart);
	}
}
	
namespace kurs
{
#if KURS_COMPILER_ID == KURS_COMPILER_GCC
	template<typename T>
	constexpr std::string_view getTypeName()
	{
		return detail::getTypeNameGnu<T>();
	}
#elif KURS_COMPILER_ID == KURS_COMPILER_CLANG
	template<typename T>
	constexpr std::string_view getTypeName()
	{
		return detail::getTypeNameClang<T>();
	}
#elif KURS_COMPILER_ID == KURS_COMPILER_MSVC
	template<typename T>
	constexpr std::string_view getTypeName()
	{
		return detail::getTypeNameMsvc<T>();
	}
#endif

}
