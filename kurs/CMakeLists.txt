add_executable(
	kursapp
	./src/Core/TypeID.cpp
	./src/Core/FormatString.cpp
	./src/Core/Timer.cpp
	./src/Debug/Assertion/AssertImpl.cpp
	./src/Debug/Logging/Logger.cpp
	./src/Debug/Logging/TimedLogFormatter.cpp
	./src/Debug/Logging/ConsoleLogWriter.cpp
	./src/Debug/Logging/FileLogWriter.cpp
	./src/GameApp/main.cpp
)

target_include_directories(
	kursapp
	PRIVATE
	./src
)

target_link_libraries(
	kursapp
	
	PRIVATE
	SDL3-static
)

target_precompile_headers(
	kursapp

	PRIVATE
	./src/kurspch.hpp
)

set_target_properties(
	kursapp
	
	PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED TRUE

	# Might be useful to measure compile and link time
	RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time"
	RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time"
)
