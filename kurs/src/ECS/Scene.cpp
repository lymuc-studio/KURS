#include "ECS/Scene.hpp"

#include "Debug/Logging/Logger.hpp"

namespace kurs
{
	Scene::Scene(std::string_view name)
		: m_Name(name)
		, m_Registry()
		, m_SharedRegistry(&m_Registry, [](auto){})
	{
	}

	std::string_view Scene::GetName() const
	{
		return m_Name;
	}

	Entity Scene::CreateEntity()
	{
		return Entity(m_SharedRegistry);
	}

	void Scene::Update(Timestep ts)
	{
		for (auto& system : m_Systems)
		{
			system->OnUpdate(ts);
		}
	}
}
