#pragma once

#include <chrono>

namespace kurs
{
	class Timestep
	{
	public:
		explicit Timestep(std::chrono::nanoseconds nanoseconds);

		float GetNanoseconds() const;
		float GetMilliseconds() const;
		float GetSeconds() const;

		explicit operator std::chrono::nanoseconds() const;

	private:
		std::chrono::nanoseconds m_Nanoseconds;
	};

	class Timer
	{
	public:
		Timer();

		void Tick();
		Timestep GetDeltaTime() const;

	private:
		std::chrono::high_resolution_clock::time_point m_Prev;
		std::chrono::nanoseconds m_Diff;
	};
}
