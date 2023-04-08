#pragma once

#include "Debug/Assertion/AssertImpl.hpp"

#include "Core/FuncName.hpp"

#ifdef NDEBUG
#	define KURS_ASSERT(expr, ...)
#else
#	define KURS_ASSERT(expr, ...) bool(expr) || ::kurs::detail::AssertImpl( \
		__FILE__,                                                             \
		__LINE__,                                                             \
		KURS_FUNCNAME,                                                        \
		#expr,                                                                \
		##__VA_ARGS__).LogMessage().DisplayMessage().DebugBreak()
#endif
