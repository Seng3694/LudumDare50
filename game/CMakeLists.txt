set(GAME_HEADERS
	include/PlayState.hpp
	include/TileMap.hpp
	include/Map1.hpp
	include/Player.hpp
	include/ResourceBar.hpp
	include/StringFormat.hpp
	include/Tiles.hpp
)

set(GAME_SOURCES
	src/Main.cpp
	src/StringFormat.cpp
	src/TileMap.cpp
)

include_directories(include)
add_executable(${PROJECT_NAME} ${GAME_HEADERS} ${GAME_SOURCES})
target_link_libraries(${PROJECT_NAME} gjt_engine)

if(EXISTS ${CMAKE_SOURCE_DIR}/content)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_SOURCE_DIR}/content
				${CMAKE_CURRENT_BINARY_DIR}/content
	)
endif()
