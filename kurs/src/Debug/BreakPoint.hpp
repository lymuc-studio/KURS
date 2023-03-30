#pragma once

#include "Core/CompilerInfo.hpp"

#if KURS_COMPILER_ID == KURS_COMPILER_GCC
#	define KURS_BREAKPOINT __builtin_trap
#elif KURS_COMPILER_ID == KURS_COMPILER_CLANG
#	if __has_builtin(__builtin_debugtrap)
#		define KURS_BREAKPOINT __builtin_debugtrap
#	else
#		error CLANG has no __builtin_debugtrap available
#	endif
#elif KURS_COMPILER_ID == KURS_COMPILER_MSVC
#	define KURS_BREAKPOINT __debugbreak
#endif
