#pragma once

#include "Event/EventBus.hpp"

namespace kurs
{
	class EventPoster
	{
	public:
		EventPoster() = default;
		EventPoster(EventBus* bus);

		EventPoster(EventPoster&& other) noexcept;

		EventPoster& operator=(EventPoster&& other) noexcept;

		void SetBus(EventBus* bus);
		EventBus* GetBus() const;
		void Reset();

		void PostEvent(EventBase& event);

		template<typename EventT, typename... ArgsT>
		void CreateAndPost(ArgsT&&... args)
		{
			if (m_Bus)
			{
				EventT eventInstance{ std::forward<ArgsT>(args)... };
				PostEvent(eventInstance);
			}
		}

	private:
		EventBus* m_Bus = nullptr;
	};
}
