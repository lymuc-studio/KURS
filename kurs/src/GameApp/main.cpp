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

#include "Memory/ObjectAllocator.hpp"
#include "Memory/StandardAllocator.hpp"

#include "Debug/BreakPoint.hpp"
#include "Debug/Assertion/Assert.hpp"
#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/ConsoleLogWriter.hpp"
#include "Debug/Logging/FileLogWriter.hpp"
#include "Debug/Logging/TimedLogFormatter.hpp"

class alignas(1024) CustomType
{
public:
	CustomType()
	{
		KURS_LOG(Info, "Instance created");
	}

	~CustomType()
	{
		KURS_LOG(Info, "Instance destroyed");
	}

	void PrintAddr()
	{
		KURS_LOG(Info, "Instance location: %p", this);
	}

private:
	char Mebibyte[1024ull * 1024 * 1024];
};

int main()
{
	kurs::Logger::Get()
		.SetFormatter(std::make_unique<kurs::TimedLogFormatter>())
		.AddWriter(std::make_unique<kurs::FileLogWriter>())
		.AddWriter(std::make_unique<kurs::ConsoleLogWriter>());

	kurs::AllocatorSpecification spec{ sizeof(CustomType), alignof(CustomType) };

	kurs::ObjectAllocator<CustomType> alloc(
		std::make_unique<kurs::StandardAllocator>(spec)
	);

	auto another = std::move(alloc);
	alloc = std::move(another);

	auto inst = alloc.CreateInstanceShared();

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
	SDL_Renderer* render = SDL_CreateRenderer(
		window, 
		nullptr, 
		SDL_RENDERER_ACCELERATED
	);
	SDL_SetRenderVSync(render, 1);

	SDL_RendererInfo renderInfo = {};
	SDL_GetRendererInfo(render, &renderInfo);

	KURS_LOG(Info, "Graphics device: %s", renderInfo.name);

	kurs::Timer timer;
	float intervalSeconds = 0.0;

	bool running = true;
	while (running)
	{
		timer.Tick();
		intervalSeconds += timer.GetDeltaTime().GetSeconds();

		alloc.CreateInstance();

		SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
		SDL_RenderClear(render);

		int windowWidth = 0;
		int windowHeight = 0;
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);

		float targetRatio = 16.0 / 9.0;
		float currentRatio = float(windowWidth) / windowHeight;

		float scale = windowWidth / 1920.0f;

		SDL_SetRenderScale(render,  scale, scale);
		
		SDL_FRect r;
		r.x = 10;
		r.y = 10;
		r.w = 150;
		r.h = 150;
		SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
		SDL_RenderFillRect(render, &r);

		if (intervalSeconds >= 2.0f)
		{
			KURS_LOG(Debug, "FPS: %f", 1.0f / timer.GetDeltaTime().GetSeconds());
			intervalSeconds = 0.0f;
		}

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
			
				if (keyCode == SDLK_b)
				{
					KURS_ASSERT(false, "You should not press the %s key", SDL_GetKeyName(keyCode));
					std::cout << "Stuff happening after assert...\n";
				}
			}
		}

		SDL_RenderPresent(render);
	}

	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_Quit();
}
