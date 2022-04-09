cmake_minimum_required(VERSION 3.5)

project(MowBot)

if(WIN32)
	set(SFML_STATIC_LIBRARIES TRUE)
endif()

add_subdirectory(engine)
add_subdirectory(game)
