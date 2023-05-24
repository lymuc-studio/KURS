#include "Scripting/ScriptSystem.hpp"

#include "Scripting/Components.hpp"

#include "Debug/Assertion/Assert.hpp"

namespace kurs
{
	void ScriptSystem::OnAttach(std::weak_ptr<detail::EntityRegistry> registry)
	{
		m_Registry = registry;
	}

	void ScriptSystem::OnDetach()
	{
		m_Registry.reset();
	}

	void ScriptSystem::OnUpdate(Timestep ts)
	{
		KURS_ASSERT(
			!m_Registry.expired(),
			"Dangling ScriptSystem"
		);
	
		m_Registry.lock()->ForEach<NativeScriptComponent>(
			[this, ts](auto eid, NativeScriptComponent& nsc)
			{
				if (!nsc.IsBound())
				{
					return;
				}
				if (!nsc.IsInstantiated())
				{
					nsc.Instantiate();
					nsc.GetInstance().OnStart();
				}
				nsc.GetInstance().BindEntity(Entity(m_Registry, eid));
				nsc.GetInstance().OnUpdate(ts);
				nsc.GetInstance().UnbindEntity();
			}
		);
	}
}
