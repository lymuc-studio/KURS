#include "Debug/Assertion/AssertImpl.hpp"

#include "Core/FormatString.hpp"
#include "Debug/Logging/Logger.hpp"
#include "Debug/BreakPoint.hpp"

#include <SDL3/SDL.h>

namespace kurs::detail
{
	AssertImpl::AssertImpl(
		std::string_view fileName,
		std::uint32_t lineNumber,
		std::string_view scopeName,
		std::string_view expression,
		std::string_view format,
		...
	)
	{
		m_Data.FileName = fileName;
		m_Data.LineNumber = lineNumber;
		m_Data.ScopeName = scopeName;
		m_Data.Expression = expression;

		va_list args;
		va_start(args, format);
		m_Data.Message = formatString(format, args);
		va_end(args);
	}

	const AssertImpl& AssertImpl::LogMessage() const
	{
		Logger::Get().MakeLog(
			LogLevel_Error,
			m_Data.FileName,
			m_Data.LineNumber,
			m_Data.ScopeName,
			"Assertion failed! %s",
			m_Data.Message.data()
		);
		
		return *this;
	}

	const AssertImpl& AssertImpl::DisplayMessage() const
	{
		auto info = formatString(
			"Assertion failure\n"
			"File: %s, %u\n"
			"Scope: %s\n"
			"Expression: %s\n\n"
			"Details: %s\n",
			m_Data.FileName.data(),
			m_Data.LineNumber,
			m_Data.ScopeName.data(),
			m_Data.Expression.data(),
			m_Data.Message.data()
		);

		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_WARNING,
			"KURS - Runtime failure",
			info.data(),
			nullptr
		);
		
		return *this;
	}

	bool AssertImpl::DebugBreak() const
	{
		KURS_BREAKPOINT();
		return false;
	}
}
