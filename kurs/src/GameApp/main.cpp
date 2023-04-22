#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/FileLogWriter.hpp"
#include "Debug/Logging/ConsoleLogWriter.hpp"
#include "Debug/Logging/TimedLogFormatter.hpp"

#include "Core/TypeName.hpp"
#include "Core/IDRegistry.hpp"

#include "Event/EventBase.hpp"
#include "Event/EventBus.hpp"
#include "Event/EventSubscriber.hpp"

struct AppStartEvent : public kurs::Event<AppStartEvent>
{
	int AppCode = 156;
};

struct AppShutDownEvent : public kurs::Event<AppShutDownEvent>
{
	int ReturnCode = 0;
};

void externalCallback(AppShutDownEvent& e)
{
	KURS_LOG(Warn, "Destroy some global state");
}

class App
{
public:
	App(kurs::EventBus* bus)
	{
		m_Subscriber.SetBus(bus);

		m_Subscriber
			.BeginThis(this)
				.AddMethod(&App::OnAppStart)
				.AddMethod(&App::OnAppShutDown)
			.EndThis()
			.AddFunction(externalCallback);
	}

private:
	void OnAppStart(AppStartEvent& e)
	{
		KURS_LOG(Warn, "Start up: %d", e.AppCode);
	}

	void OnAppShutDown(AppShutDownEvent& e)
	{
		KURS_LOG(Warn, "Shut down: %d", e.ReturnCode);
	}

	kurs::EventSubscriber m_Subscriber;
};

int main()
{
	kurs::LogLevel verbosity = kurs::LogLevel_Debug;

	kurs::Logger::Get()
		.AddWriter(std::make_unique<kurs::FileLogWriter>("kurs.log"))
		.AddWriter(std::make_unique<kurs::ConsoleLogWriter>())
		.SetFormatter(std::make_unique<kurs::TimedLogFormatter>(verbosity));

	kurs::EventBus mainBus;

	auto inst = new App(&mainBus);

	AppStartEvent start;
	mainBus.Publish(start);

	AppShutDownEvent shutdown;
	mainBus.Publish(shutdown);

	delete inst;

	mainBus.Publish(start);

	mainBus.Publish(shutdown);
}
