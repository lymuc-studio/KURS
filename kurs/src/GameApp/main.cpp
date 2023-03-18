#include <cstdlib>

#include <iostream>

#include <SDL3/SDL.h>

#include "kurspch.h"

#include "Core/CompilerInfo.hpp"
#include "Core/FuncName.hpp"
#include "Core/TypeName.hpp"

int main()
{
	if (0 != SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"KURS",
			"Failed to initialize SDL3",
			nullptr
		);

		return EXIT_FAILURE;
	}

	std::cout << kurs::getCompilerName() << " edition" << std::endl;
	std::cout << kurs::getTypeName<std::string>() << std::endl;

	SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_INFORMATION,
		"KURS",
		"Hello KURS!  |  Привет КУРС!",
		nullptr
	);

	SDL_Window* window = SDL_CreateWindow(
		"KURS - Game",
		1280,
		720,
		SDL_WINDOW_RESIZABLE
	);

	bool running = true;
	while (running)
	{
		for (SDL_Event event{}; SDL_PollEvent(&event);)
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				running = false;
			}
		}
	}

	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_Quit();
}
