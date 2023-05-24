#pragma once

#include "Core/Timer.hpp"

#include "ECS/Entity.hpp"

namespace kurs
{
	class NativeScript
	{
	public:
		virtual ~NativeScript() = default;

		void BindEntity(Entity entity);
		void UnbindEntity();

		template<typename ComponentT>
		bool HasComponent() const
		{
			return m_Entity.HasComponent<ComponentT>();
		}

		template<typename... ComponentsT>
		bool HasAllOf() const
		{
			return m_Entity.HasAllOf<ComponentsT...>();
		}

		template<typename ComponentT, typename... ArgsT>
		ComponentT& AddComponent(ArgsT&&... args)
		{
			return m_Entity.AddComponent<ComponentT>(std::forward<ArgsT>(args)...);
		}

		template<typename ComponentT>
		ComponentT& GetComponent()
		{
			return m_Entity.GetComponent<ComponentT>();
		}

		template<typename ComponentT>
		void RemoveComponent()
		{
			return m_Entity.RemoveComponent<ComponentT>();
		}

		void RemoveAllComponents();

		virtual void OnStart() {}
		virtual void OnUpdate(Timestep ts) {}

	private:
		Entity m_Entity;
	};
}
