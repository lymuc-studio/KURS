#include "Game/App.hpp"

#include "Scripting/Components.hpp"
#include "Scripting/ScriptSystem.hpp"
#include "Game/FpsDisplayScript.hpp"

namespace kurs
{
	App::App()
	{
		m_MainScene.AttachSystem<ScriptSystem>();

		Entity fpsDisplay = m_MainScene.CreateEntity();
		
		fpsDisplay.AddComponent<NativeScriptComponent>(
			kurs::BindScript<FpsDisplayScript>()
		);
		
		m_Entities.push_back(fpsDisplay);
	}

	void App::Run()
	{
		CreateWindow();

		m_Running = true;
		while (m_Running)
		{
			m_FrameTimer.Tick();
			Update(m_FrameTimer.GetDeltaTime());
		}
	}

	void App::Quit()
	{
		m_Running = false;
		DestroyWindow();
	}

	void App::Update(Timestep ts)
	{
		PollEvents();
		m_MainScene.Update(ts);
	}

	void App::CreateWindow()
	{
		m_Window = SDL_CreateWindow(
			m_MainScene.GetName().data(),
			1280,
			720,
			0
		);
	}

	void App::DestroyWindow()
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}

	void App::PollEvents()
	{
		for (SDL_Event e{}; SDL_PollEvent(&e);)
		{
			if (SDL_EVENT_QUIT == e.type)
			{
				Quit();
			}
			if (SDL_EVENT_KEY_DOWN == e.type)
			{
				if (SDL_SCANCODE_LCTRL == e.key.keysym.scancode)
				{
					KURS_LOG(Info, "Destroying entities");
					DestroyEntities();
				}
			}
		}
	}

	void App::DestroyEntities()
	{
		m_Entities.clear();
	}
}
