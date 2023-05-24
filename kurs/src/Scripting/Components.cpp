#include "Scripting/Components.hpp"

namespace kurs
{
	void NativeScriptComponent::Unbind()
	{
		m_Instantiate = nullptr;
	}

	bool NativeScriptComponent::IsBound() const
	{
		return bool(m_Instantiate);
	}

	void NativeScriptComponent::Instantiate()
	{
		m_Instance = m_Instantiate();
	}

	void NativeScriptComponent::Destroy()
	{
		m_Instance.reset();
	}

	bool NativeScriptComponent::IsInstantiated() const
	{
		return bool(m_Instance);
	}

	NativeScript& NativeScriptComponent::GetInstance()
	{
		return *m_Instance;
	}
}
