add_executable(
	kursapp
	./src/GameApp/main.cpp
)

set_target_properties(
	kursapp
	
	PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED TRUE
)
