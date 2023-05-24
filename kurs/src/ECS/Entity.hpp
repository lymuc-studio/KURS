#pragma once

#include "ECS/EntityRegistry.hpp"

#include "Debug/Assertion/Assert.hpp"

namespace kurs
{
	class Entity
	{
	public:
		Entity() = default;

		Entity(std::weak_ptr<detail::EntityRegistry> registry);
		
		Entity(
			std::weak_ptr<detail::EntityRegistry> registry,
			detail::EntityID existingEntityID
		);

		~Entity();

		Entity(const Entity& other);
		Entity(Entity&& other) noexcept;

		Entity& operator=(const Entity& other);
		Entity& operator=(Entity&& other) noexcept;

		template<typename ComponentT>
		bool HasComponent() const
		{
			KURS_ASSERT(
				CheckEntityValid(),
				"Could not check whether '%s' component exists:"
				" trying to access invalid entity",
				getTypeName<ComponentT>().data()
			);
			return GetValidRegistry().HasComponent<ComponentT>(m_ID);
		}

		template<typename... ComponentsT>
		bool HasAllOf() const
		{
			KURS_ASSERT(
				CheckEntityValid(),
				"Could not check whether components exist:"
				" trying to access invalid entity"
			);
			return GetValidRegistry().HasAllOf<ComponentsT...>(m_ID);
		}

		template<typename ComponentT, typename... ArgsT>
		ComponentT& AddComponent(ArgsT&&... args)
		{
			KURS_ASSERT(
				CheckEntityValid(),
				"Could not add '%s' component: trying to modify invalid entity",
				getTypeName<ComponentT>().data()
			);
			
			return GetValidRegistry().EmplaceComponent<ComponentT>(
				m_ID, 
				std::forward<ArgsT>(args)...
			);
		}

		template<typename ComponentT, typename... ArgsT>
		ComponentT& ReplaceComponent(ArgsT&&... args)
		{
			KURS_ASSERT(
				CheckEntityValid(),
				"Could not replace '%s' component: trying to modify invalid entity",
				getTypeName<ComponentT>().data()
			);
			
			return GetValidRegistry().ReplaceComponent<ComponentT>(
				m_ID,
				std::forward<ArgsT>(args)...
			);
		}

		template<typename ComponentT>
		ComponentT& GetComponent()
		{
			KURS_ASSERT(
				CheckEntityValid(),
				"Could not get '%s' component: trying to modify invalid entity",
				getTypeName<ComponentT>().data()
			);
			return GetValidRegistry().GetComponent<ComponentT>(m_ID);
		}

		template<typename ComponentT>
		void RemoveComponent()
		{
			KURS_ASSERT(
				CheckEntityValid(),
				"Could not remove '%s' component: trying to modify invalid entity",
				getTypeName<ComponentT>().data()
			);
			GetValidRegistry().RemoveComponent<ComponentT>(m_ID);
		}

		void RemoveAllComponents();

		operator bool() const;

	private:
		void IncrementRefCount();
		void DecrementRefCount();

		bool CheckEntityValid() const;
		bool CheckEntityDangling() const;

		detail::EntityRegistry& GetValidRegistry() const;

		std::weak_ptr<detail::EntityRegistry> m_Registry;
		detail::EntityID m_ID = detail::EntityRegistry::c_NullEntityID;
	};
}
