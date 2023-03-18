#pragma once

#include "kurspch.hpp"

#include "Core/CompilerInfo.hpp"
	
#if KURS_COMPILER_ID == KURS_COMPILER_GCC
#	define KURS_FUNCNAME __PRETTY_FUNCTION__
#elif KURS_COMPILER_ID == KURS_COMPILER_CLANG
#	define KURS_FUNCNAME __PRETTY_FUNCTION__
#elif KURS_COMPILER_ID == KURS_COMPILER_MSVC
#	define KURS_FUNCNAME __FUNCSIG__
#endif

#ifndef KURS_FUNCNAME
#	define KURS_FUNCNAME __func__
#endif
