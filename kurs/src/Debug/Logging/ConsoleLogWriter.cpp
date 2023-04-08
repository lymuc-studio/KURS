#include "Debug/Logging/ConsoleLogWriter.hpp"

#include <iostream>
#include <mutex>

namespace kurs
{
	void ConsoleLogWriter::WriteLine(std::string_view message)
	{
		static std::mutex s_ConsoleMutex;

		if (message.size())
		{
			std::lock_guard consoleGuard(s_ConsoleMutex);
			std::cout << message << std::endl;
		}
	}
}
