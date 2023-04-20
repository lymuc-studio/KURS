#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/FileLogWriter.hpp"
#include "Debug/Logging/ConsoleLogWriter.hpp"
#include "Debug/Logging/TimedLogFormatter.hpp"

#include "Core/TypeName.hpp"

int main()
{
	kurs::LogLevel verbosity = kurs::LogLevel_Debug;

	kurs::Logger::Get()
		.AddWriter(std::make_unique<kurs::FileLogWriter>("kurs.log"))
		.AddWriter(std::make_unique<kurs::ConsoleLogWriter>())
		.SetFormatter(std::make_unique<kurs::TimedLogFormatter>(verbosity));

	std::string intName(kurs::getTypeName<int>());

	KURS_LOG(Info, "%s", intName.data());
}
