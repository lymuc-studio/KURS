#pragma once

#include "Core/Timer.hpp"

#include "Event/EventSubscriber.hpp"
#include "Event/EventPoster.hpp"

#include <SDL3/SDL.h>

namespace kurs
{
	struct AppRunEvent : kurs::Event<AppRunEvent>
	{
		AppRunEvent(class App* inst)
			: Instance(inst) 
		{}

		class App* Instance = nullptr;
	};

	struct AppQuitEvent : kurs::Event<AppQuitEvent>
	{
		AppQuitEvent(class App* inst)
			: Instance(inst)
		{}

		class App* Instance = nullptr;
	};

	class App
	{
	public:
		App(EventBus* bus);
	
		void Run();
		void Quit();
		bool IsRunning() const;
		void Update(Timestep ts);

	private:
		void OnRun(AppRunEvent& event);
		void OnQuit(AppQuitEvent& event);

		bool m_Running = false;

		EventSubscriber m_Subscriber;
		EventPoster m_Poster;
	
		Timer m_FrameTimer;
	
		SDL_Window* m_Window = nullptr;
	};
}
