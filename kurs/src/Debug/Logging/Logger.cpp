#include "Debug/Logging/Logger.hpp"

#include "Core/FormatString.hpp"

namespace kurs
{
	Logger& Logger::Get()
	{
		static Logger s_Instance;
		return s_Instance;
	}

	void Logger::MakeLog(
		LogLevel level,
		std::string_view fileName,
		std::uint32_t lineNumber,
		std::string_view scopeName,
		std::string_view format,
		...
	)
	{
		LogData data;
		data.Level = level;
		data.FileName = fileName;
		data.LineNumber = lineNumber;
		data.ScopeName = scopeName;

		va_list args;
		va_start(args, format);
		data.Message = formatString(format, args);
		va_end(args);
	
		if (m_Formatter && !m_Writer.empty())
		{
			auto logLine = m_Formatter->FormatData(data);
			for (auto& writer : m_Writer)
			{
				writer->WriteLine(logLine);
			}
		}
	}

	Logger& Logger::SetFormatter(std::unique_ptr<ILogFormatter> formatter)
	{
		m_Formatter = std::move(formatter);
		return *this;
	}

	Logger& Logger::AddWriter(std::unique_ptr<ILogWriter> writer)
	{
		if (writer)
		{
			m_Writer.emplace_back(std::move(writer));
		}

		return *this;
	}
}
