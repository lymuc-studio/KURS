#include "Game/App.hpp"

namespace kurs
{
	App::App(EventBus* bus)
		: m_Subscriber(bus)
		, m_Poster(bus)
	{
		m_Subscriber
			.BeginThis(this)
				.AddMethod(&App::OnRun)
				.AddMethod(&App::OnQuit)
			.EndThis();
	}

	void App::Run()
	{
		m_Running = true;

		m_Poster.CreateAndPost<AppRunEvent>(this);
	
		while (IsRunning())
		{
			m_FrameTimer.Tick();
			Update(m_FrameTimer.GetDeltaTime());
		}
	}
	
	void App::Quit()
	{
		m_Poster.CreateAndPost<AppQuitEvent>(this);
		m_Running = false;
	}
	
	bool App::IsRunning() const
	{
		return m_Running;
	}

	void App::Update(Timestep ts)
	{
		for (SDL_Event event; SDL_PollEvent(&event);)
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				Quit();
			}
		}
	}

	void App::OnRun(AppRunEvent& event)
	{
		m_Window = SDL_CreateWindow(
			"Game",
			1280,
			720,
			SDL_WINDOW_VULKAN
		);
	
		KURS_ASSERT(m_Window, "Failed to create window");
	}

	void App::OnQuit(AppQuitEvent& event)
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}
}
