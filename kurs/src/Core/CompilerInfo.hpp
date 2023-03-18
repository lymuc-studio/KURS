#pragma once

#include "kurspch.hpp"

#define KURS_COMPILER_GCC   0
#define KURS_COMPILER_CLANG 1
#define KURS_COMPILER_MSVC  2

#if defined(__GNUG__) && !defined(__clang__)
#	define KURS_COMPILER_ID KURS_COMPILER_GCC
#elif defined(__clang__)
#	define KURS_COMPILER_ID KURS_COMPILER_CLANG
#elif defined(_MSC_VER)
#	define KURS_COMPILER_ID KURS_COMPILER_MSVC
#endif

#ifndef KURS_COMPILER_ID
#	error Unknown compiler
#endif

namespace kurs
{
	constexpr std::string_view getCompilerName()
	{
		constexpr std::string_view compilerNames[]
		{
			"GCC",
			"CLANG",
			"MSVC"
		};

		return compilerNames[KURS_COMPILER_ID];
	}
}
