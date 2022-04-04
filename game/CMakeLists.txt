set(GAME_HEADERS
	include/AudioFiles.hpp
	include/PlayState.hpp
	include/LDGame.hpp
	include/TileMap.hpp
	include/Line.hpp
	include/Maps.hpp
	include/MapView.hpp
	include/MapSelectionState.hpp
	include/Player.hpp
	include/ResourceBar.hpp
	include/SaveFileManager.hpp
	include/StringFormat.hpp
	include/Score.hpp
	include/Tiles.hpp
	include/TitleState.hpp
	include/TransitionState.hpp
	include/VictoryState.hpp
	include/VolumeBar.hpp
)

set(GAME_SOURCES
	src/Main.cpp
	src/Maps.cpp
	src/MapView.cpp
	src/SaveFileManager.cpp
	src/StringFormat.cpp
	src/TileMap.cpp
	src/VolumeBar.cpp
)

include_directories(include)
add_executable(${PROJECT_NAME} ${GAME_HEADERS} ${GAME_SOURCES} "${CMAKE_SOURCE_DIR}/content/res.rc")
target_link_libraries(${PROJECT_NAME} gjt_engine)

if(EXISTS ${CMAKE_SOURCE_DIR}/content)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_SOURCE_DIR}/content
				${CMAKE_CURRENT_BINARY_DIR}/content
	)
endif()

add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SFML_DIR}/../../../bin/openal32.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)