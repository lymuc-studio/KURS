#include "Core/Timer.hpp"

namespace kurs
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// Timestep
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	Timestep::Timestep(std::chrono::nanoseconds nanoseconds)
		: m_Nanoseconds(nanoseconds)
	{
	}

	float Timestep::GetNanoseconds() const
	{
		return static_cast<float>(m_Nanoseconds.count());
	}

	float Timestep::GetMilliseconds() const
	{
		constexpr float c_MilliOverNano = 1e6f;
		return GetNanoseconds() * c_MilliOverNano;
	}

	float Timestep::GetSeconds() const
	{
		const float c_Nano = 1e-9f;
		return GetNanoseconds() * c_Nano;
	}
	
	Timestep::operator std::chrono::nanoseconds() const
	{
		return m_Nanoseconds;
	}

	
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// Timer
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	Timer::Timer()
	{
		Tick();
	}

	void Timer::Tick()
	{
		auto now = std::chrono::high_resolution_clock::now();
		m_Diff = now - m_Prev;
		m_Prev = now;
	}

	Timestep Timer::GetDeltaTime() const
	{
		return Timestep(m_Diff);
	}
}
