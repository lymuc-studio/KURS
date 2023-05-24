#pragma once

#include "kurspch.hpp"

#include "Core/Timer.hpp"

#include "ECS/Scene.hpp"

#include <SDL3/SDL.h>

namespace kurs
{
	class App
	{
	public:
		App();
		~App() = default;

		void Run();
		void Quit();
		void Update(Timestep ts);

	private:
		void CreateWindow();
		void DestroyWindow();
		void PollEvents();
		void DestroyEntities();

		bool m_Running = false;
		
		Timer m_FrameTimer;

		Scene m_MainScene;
		std::vector<Entity> m_Entities;

		SDL_Window* m_Window = nullptr;
	};
}
