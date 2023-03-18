#include "Debug/Logging/TimedLogFormatter.hpp"

#include <ctime>

#include <sstream>
#include <filesystem>

#include "Core/FormatString.hpp"

namespace kurs
{
	TimedLogFormatter::TimedLogFormatter(LogLevel severityLevel)
		: m_SeverityLevel(severityLevel)
	{
	}

	std::string TimedLogFormatter::FormatData(const LogData& data)
	{
		if (data.Level < m_SeverityLevel)
		{
			return std::string();
		}

		constexpr std::array<std::string_view, LogLevel_Count_> levelRepr
		{
			"[debug]",
			"[trace]",
			"[info ]",
			"[warn ]",
			"[error]",
		};

		namespace fs = std::filesystem;
		std::string fileName = fs::path(data.FileName).filename().string();
	
		std::time_t currentTime = std::time(nullptr);
		std::tm* localCurrentTime = std::localtime(&currentTime);

		int hour = localCurrentTime->tm_hour;
		int minute = localCurrentTime->tm_min;
		int second = localCurrentTime->tm_sec;

		return formatString(
			"%s %.2d:%.2d:%.2d <%s, %d> %s :: %s",
			levelRepr[data.Level].data(),
			hour,
			minute,
			second,
			fileName.data(),
			data.LineNumber,
			data.ScopeName.data(),
			data.Message.data()
		);
	}
}
