#include <cstdlib>

#include <iostream>

#include <SDL3/SDL.h>

#include "kurspch.hpp"

#include "Core/CompilerInfo.hpp"
#include "Core/FuncName.hpp"
#include "Core/TypeName.hpp"
#include "Core/TypeID.hpp"
#include "Core/FormatString.hpp"
#include "Core/Timer.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/ConsoleLogWriter.hpp"
#include "Debug/Logging/FileLogWriter.hpp"
#include "Debug/Logging/TimedLogFormatter.hpp"

int main()
{
	kurs::Logger::Get()
		.SetFormatter(std::make_unique<kurs::TimedLogFormatter>())
		.AddWriter(std::make_unique<kurs::FileLogWriter>())
		.AddWriter(std::make_unique<kurs::ConsoleLogWriter>());

	if (0 != SDL_Init(SDL_INIT_EVERYTHING))
	{
		KURS_LOG(Warn, "Failed to initialize %s: %s", "SDL3", SDL_GetError());

		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_WARNING,
			"KURS",
			"Failed to initialize SDL3",
			nullptr
		);
	}

	KURS_LOG(Info, "Compiler: %s", kurs::getCompilerName().data());

	SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_INFORMATION,
		"KURS",
		"EN: Hello KURS!\nRU: Привет, КУРС!",
		nullptr
	);

	SDL_Window* window = SDL_CreateWindow(
		"KURS - Game",
		1280,
		720,
		SDL_WINDOW_RESIZABLE
	);

	kurs::Timer timer;

	bool running = true;
	while (running)
	{
		timer.Tick();

		KURS_LOG(Debug, "FPS: %f", 1.0f / timer.GetDeltaTime().GetSeconds());

		for (SDL_Event event{}; SDL_PollEvent(&event);)
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				KURS_LOG(Info, "Window close event");
				running = false;
			}

			if (event.type == SDL_EVENT_KEY_DOWN)
			{
				SDL_Keycode keyCode = SDL_GetKeyFromScancode(event.key.keysym.scancode);
				KURS_LOG(Debug, "Key pressed: %s", SDL_GetKeyName(keyCode));
			}
		}
	}

	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_Quit();
}
