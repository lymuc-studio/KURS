#include "Scripting/NativeScript.hpp"

namespace kurs
{
	void NativeScript::BindEntity(Entity entity)
	{
		m_Entity = entity;
	}

	void NativeScript::UnbindEntity()
	{
		m_Entity = Entity();
	}

	void NativeScript::RemoveAllComponents()
	{
		m_Entity.RemoveAllComponents();
	}
}
