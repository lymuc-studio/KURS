#include "ECS/Entity.hpp"

#include "Debug/Logging/Logger.hpp"

namespace kurs
{
	Entity::Entity(std::weak_ptr<detail::EntityRegistry> registry)
		: m_Registry(registry)
		, m_ID(
				registry.expired()
				? detail::EntityRegistry::c_NullEntityID 
				: registry.lock()->CreateEntity()
			)
	{
	}

	Entity::Entity(
		std::weak_ptr<detail::EntityRegistry> registry, 
		detail::EntityID existingEntityID
	)
		: m_Registry(registry)
		, m_ID(existingEntityID)
	{
		KURS_ASSERT(
			CheckEntityValid(), 
			"Trying to reference null-entity that cannot exist"
		);
		IncrementRefCount();
	}

	Entity::Entity(const Entity& other)
		: m_Registry(other.m_Registry)
		, m_ID(other.m_ID)
	{
		IncrementRefCount();
	}

	Entity::Entity(Entity&& other) noexcept
		: m_Registry(std::move(other.m_Registry))
		, m_ID(std::exchange(other.m_ID, detail::EntityRegistry::c_NullEntityID))
	{
	}

	Entity::~Entity()
	{
		DecrementRefCount();
	}

	Entity& Entity::operator=(const Entity& other)
	{
		if (this == &other)
		{
			return *this;
		}

		DecrementRefCount();
		m_Registry = other.m_Registry;
		m_ID = other.m_ID;
		IncrementRefCount();

		return *this;
	}

	Entity& Entity::operator=(Entity&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DecrementRefCount();
		m_Registry = std::move(other.m_Registry);
		m_ID = std::exchange(other.m_ID, detail::EntityRegistry::c_NullEntityID);
	
		return *this;
	}

	void Entity::RemoveAllComponents()
	{
		KURS_ASSERT(
			CheckEntityValid(), 
			"Could not remove all components: trying to modify invalid entity"
		);
		GetValidRegistry().RemoveAllComponents(m_ID);
	}

	Entity::operator bool() const
	{
		return CheckEntityValid();
	}

	void Entity::IncrementRefCount()
	{
		if (CheckEntityValid())
		{
			GetValidRegistry().IncEntityRefCount(m_ID);
		}
	}

	void Entity::DecrementRefCount()
	{
		if (CheckEntityValid())
		{
			GetValidRegistry().ReleaseEntity(m_ID);
			m_ID = detail::EntityRegistry::c_NullEntityID;
		}
	}

	bool Entity::CheckEntityValid() const
	{
		return detail::EntityRegistry::c_NullEntityID != m_ID;
	}
	
	bool Entity::CheckEntityDangling() const
	{
		return CheckEntityValid()
			&& m_Registry.expired();
	}

	kurs::detail::EntityRegistry& Entity::GetValidRegistry() const
	{
		KURS_ASSERT(
			!CheckEntityDangling(), 
			"Dangling entity has tried to obtain a reference to a registry. "
			"The order of entity and registry destruction should be reconsidered"
		);
		KURS_ASSERT(
			m_Registry.lock()->CheckEntityExists(m_ID),
			"Entity with ID %zu does not exist",
			m_ID
		);
		
		return *m_Registry.lock();
	}
}
