#pragma once

#include "kurspch.hpp"

#include "Core/FuncName.hpp"

#ifdef NDEBUG
#	define KURS_LOG(level, format, ...)
#else
#	define KURS_LOG(level, format, ...) ::kurs::Logger::Get().MakeLog( \
	::kurs::LogLevel_##level,                                          \
	__FILE__,                                                          \
	__LINE__,                                                          \
	KURS_FUNCNAME,                                                     \
	format,                                                            \
	##__VA_ARGS__)
#endif

namespace kurs
{
	enum LogLevel
	{
		LogLevel_Debug,
		LogLevel_Trace,
		LogLevel_Info,
		LogLevel_Warn,
		LogLevel_Error,

		LogLevel_Count_,
	};

	struct LogData
	{
		LogLevel Level;
		std::string FileName;
		std::uint32_t LineNumber;
		std::string ScopeName;
		std::string Message;
	};

	class ILogFormatter
	{
	public:
		virtual ~ILogFormatter() = default;

		virtual std::string FormatData(const LogData& data) = 0;
	};

	class ILogWriter
	{
	public:
		virtual ~ILogWriter() = default;

		virtual void WriteLine(std::string_view message) = 0;
	};

	class Logger
	{
	public:
		static Logger& Get();

		void MakeLog(
			LogLevel level,
			std::string_view fileName,
			std::uint32_t lineNumber,
			std::string_view scopeName,
			std::string_view format,
			...
		);

		Logger& SetFormatter(std::unique_ptr<ILogFormatter> formatter);
		Logger& SetWriter(std::unique_ptr<ILogWriter> writer);

	private:
		Logger() = default;
		Logger(const Logger& other) = delete;

		std::unique_ptr<ILogFormatter> m_Formatter;
		std::unique_ptr<ILogWriter> m_Writer;
	};
}
