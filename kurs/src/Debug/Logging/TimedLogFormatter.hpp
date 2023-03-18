#pragma once

#include "kurspch.hpp"

#include "Debug/Logging/Logger.hpp"

namespace kurs
{
	class TimedLogFormatter : public ILogFormatter
	{
	public:
		TimedLogFormatter(LogLevel severityLevel = LogLevel_Debug);
	
		std::string FormatData(const LogData& data) override;

	private:
		LogLevel m_SeverityLevel = LogLevel_Debug;
	};
}
