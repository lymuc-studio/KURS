#pragma once

#include "kurspch.hpp"

#include "Core/Timer.hpp"

#include "ECS/Entity.hpp"
#include "ECS/EntitySystem.hpp"
#include "ECS/EntityRegistry.hpp"

#include "Debug/Logging/Logger.hpp"

namespace kurs
{
	class Scene
	{
	public:
		Scene(std::string_view name = "Unnamed scene");

		std::string_view GetName() const;

		Entity CreateEntity();

		template<typename SystemT>
		bool HasSystem() const
		{
			TypeID systemTypeID = getTypeID<SystemT>();

			return systemTypeID < m_Systems.size()
				&& m_Systems[systemTypeID];
		}

		template<typename SystemT, typename... ArgsT>
		void AttachSystem(ArgsT&&... args)
		{
			TypeID systemTypeID = getTypeID<SystemT>();
		
			if (systemTypeID >= m_Systems.size())
			{
				m_Systems.resize(systemTypeID + 1);
			}
		
			auto& system = m_Systems[systemTypeID] = std::make_unique<SystemT>(
				std::forward<ArgsT>(args)...
			);
			system->OnAttach(m_SharedRegistry);
		}

		template<typename SystemT>
		void DetachSystem()
		{
			TypeID systemTypeID = getTypeID<SystemT>();
		
			if (!HasSystem<SystemT>())
			{
				KURS_LOG(
					Warn,
					"Scene '%s' could not detach '%s' system: system does not exist",
					m_Name.data(),
					getTypeName<SystemT>().data()
				);
				return;
			}
		
			std::unique_ptr<EntitySystemBase>& system = m_Systems[systemTypeID];
			system->OnDetach();
			system.reset();
		}

		template<typename SystemT>
		SystemT& GetSystem()
		{
			TypeID systemTypeID = getTypeID<SystemT>();
		
			KURS_ASSERT(
				HasSystem<SystemT>(),
				"Scene '%s' could not find '%s' system: system does not exist",
				m_Name.data(),
				getTypeName<SystemT>().data()
			);
		
			return *detail::entity_system_cast<SystemT>(
				m_Systems[systemTypeID].get()
			);
		}

		void Update(Timestep ts);

	private:
		std::string m_Name;

		detail::EntityRegistry m_Registry;
		std::shared_ptr<detail::EntityRegistry> m_SharedRegistry;
	
		std::vector<std::unique_ptr<EntitySystemBase>> m_Systems;
	};
}
