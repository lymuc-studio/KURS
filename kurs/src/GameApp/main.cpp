#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/FileLogWriter.hpp"
#include "Debug/Logging/ConsoleLogWriter.hpp"
#include "Debug/Logging/TimedLogFormatter.hpp"

#include "Core/TypeName.hpp"
#include "Core/IDRegistry.hpp"

int main()
{
	kurs::LogLevel verbosity = kurs::LogLevel_Debug;

	kurs::Logger::Get()
		.AddWriter(std::make_unique<kurs::FileLogWriter>("kurs.log"))
		.AddWriter(std::make_unique<kurs::ConsoleLogWriter>())
		.SetFormatter(std::make_unique<kurs::TimedLogFormatter>(verbosity));

	kurs::IDRegistry registry;

	auto id1 = registry.GenerateID();
	auto id2 = registry.GenerateID();
	auto id3 = registry.GenerateID();
	
	registry.DestroyID(id3);
	registry.DestroyID(id2);
	// registry.DestroyID(id1);

	auto id4 = registry.GenerateID();
	auto id5 = registry.GenerateID();
	auto id6 = registry.GenerateID();

	KURS_LOG(Info, "id4 = %zu", id4);
	KURS_LOG(Info, "id5 = %zu", id5);
	KURS_LOG(Info, "id6 = %zu", id6);

	registry.DestroyID(id6);
	registry.DestroyID(id5);
	registry.DestroyID(id4);
	registry.DestroyID(id3);
	registry.DestroyID(id2);
	registry.DestroyID(id1);
}
