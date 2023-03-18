#include "Debug/Logging/ConsoleLogWriter.hpp"

#include <iostream>

namespace kurs
{
	void ConsoleLogWriter::WriteLine(std::string_view message)
	{
		if (message.size())
		{
			std::cout << message << std::endl;
		}
	}
}
