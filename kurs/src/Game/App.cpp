#include "Game/App.hpp"

#include <iostream>

namespace kurs
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent(std::string_view tag)
			: Tag(tag)
		{
			std::cout << "Created: " << tag << std::endl;
		}

		~TagComponent()
		{
			std::cout << "Destroyed: " << Tag << std::endl;
		}

		void Print() const
		{
			std::cout << "TagComponent: " << Tag << std::endl;
		}
	};

	struct TransformComponent
	{
		float PosX = 0.0f;
		float PosY = 0.0f;

		TransformComponent(float posx, float posy)
			: PosX(posx)
			, PosY(posy)
		{
			std::cout << "Created: " << posx << ", " << posy << std::endl;
		}

		~TransformComponent()
		{
			std::cout << "Destroyed: " << PosX << ", " << PosY << std::endl;
		}

		void Display() const
		{
			std::cout << "TransformComponent: " << PosX << ", " << PosY << std::endl;
		}
	};

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
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
			{
				if (SDL_GetWindowID(m_Window) == event.window.windowID)
				{
					Quit();
				}
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
	
		TestECS();
	}

	void App::OnQuit(AppQuitEvent& event)
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}

	void App::TestECS()
	{
		auto e1 = m_Entities.CreateEntity();
		m_Entities.EmplaceComponent<TagComponent>(e1, "First entity");
		m_Entities.EmplaceComponent<TransformComponent>(e1, 5.f, 6.f);

		auto e2 = m_Entities.CreateEntity();
		m_Entities.EmplaceComponent<TagComponent>(e2, "Second entity");
		m_Entities.EmplaceComponent<TransformComponent>(e2, 56.f, 65.f);
		m_Entities.EmplaceComponent<int>(e2, 105);

		m_Entities.ForEach<TagComponent>(
			[this](auto eid, auto& tag)
			{
				if (m_Entities.HasAllOf<int, TransformComponent>(eid))
				{
					auto& transform = m_Entities.GetComponent<TransformComponent>(eid);
					auto& i = m_Entities.GetComponent<int>(eid);
					
					std::cout
						<< eid << ":\n"
						<< '\t' << i << '\n';
					transform.Display();

					return;
				}
				std::cout << eid << " does not have both transform and int\n";
			}
		);

		m_Entities.ReleaseEntity(e1);
		m_Entities.ReleaseEntity(e2);
	}
}
