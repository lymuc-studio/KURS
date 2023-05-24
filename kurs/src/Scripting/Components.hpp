#pragma once

#include "kurspch.hpp"

#include "Scripting/NativeScript.hpp"

namespace kurs
{
	template<typename ScriptT>
	struct BindScript {};

	struct NativeScriptComponent
	{
		NativeScriptComponent() = default;

		template<typename ScriptT>
		NativeScriptComponent(BindScript<ScriptT>)
			: NativeScriptComponent()
		{
			Bind<ScriptT>();
		}

		template<typename ScriptT>
		void Bind()
		{
			m_Instantiate = []() -> std::unique_ptr<NativeScript>
			{
				return std::make_unique<ScriptT>();
			};
		}
		void Unbind();
		bool IsBound() const;
		
		void Instantiate();
		void Destroy();
		bool IsInstantiated() const;

		NativeScript& GetInstance();

	private:
		std::unique_ptr<NativeScript>(*m_Instantiate)() = nullptr;
		std::unique_ptr<NativeScript> m_Instance;
	};
}
