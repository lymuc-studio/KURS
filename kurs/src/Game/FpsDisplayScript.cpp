#include "Game/FpsDisplayScript.hpp"

#include "Scripting/Components.hpp"
#include "Debug/Logging/Logger.hpp"

namespace kurs
{
	FpsDisplayScript::~FpsDisplayScript()
	{
		KURS_LOG(Debug, "Destroyed");
	}

	void FpsDisplayScript::OnStart()
	{
		KURS_LOG(Debug, "Instantiated");
	}

	void FpsDisplayScript::OnUpdate(Timestep ts)
	{
		if (m_ElapsedSeconds >= m_PeriodSeconds)
		{
			KURS_LOG(Debug, "FPS: %.2f", 1.0f / ts.GetSeconds());
			m_ElapsedSeconds = 0.0f;
			return;
		}
		
		m_ElapsedSeconds += ts.GetSeconds();
	}
}
