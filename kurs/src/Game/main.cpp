#include <SDL3/SDL.h>

#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/ConsoleLogWriter.hpp"
#include "Debug/Logging/FileLogWriter.hpp"
#include "Debug/Logging/TimedLogFormatter.hpp"

#include "Game/App.hpp"

int main()
{
	kurs::LogLevel verbosity = kurs::LogLevel_Debug;

	kurs::Logger::Get()
		.SetFormatter(std::make_unique<kurs::TimedLogFormatter>(verbosity))
		.AddWriter(std::make_unique<kurs::ConsoleLogWriter>())
		.AddWriter(std::make_unique<kurs::FileLogWriter>("kurs.log"));

	SDL_Init(SDL_INIT_EVERYTHING);
	{
	
		kurs::App app;
		app.Run();
	}
	SDL_Quit();
}
