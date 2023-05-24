#pragma once

#include "ECS/EntitySystem.hpp"

namespace kurs
{
	class ScriptSystem : public EntitySystem<ScriptSystem>
	{
	public:
		void OnAttach(std::weak_ptr<detail::EntityRegistry> registry) override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;

	private:
		std::weak_ptr<detail::EntityRegistry> m_Registry;
	};
}
