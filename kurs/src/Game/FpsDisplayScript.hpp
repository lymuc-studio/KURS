#pragma once

#include "Scripting/NativeScript.hpp"

namespace kurs
{
	class FpsDisplayScript : public NativeScript
	{
	public:
		~FpsDisplayScript() override;

		void OnStart() override;
		void OnUpdate(Timestep ts) override;
	
	private:
		float m_PeriodSeconds = 5.0f;
		float m_ElapsedSeconds = m_PeriodSeconds;
	};
}
