add_executable(
	kursapp
	./src/GameApp/main.cpp
)

target_link_libraries(
	kursapp
	
	PRIVATE
	SDL3-static
)

set_target_properties(
	kursapp
	
	PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED TRUE
)
