set(ENGINE_HEADERS
	include/AnimatedSprite.hpp
	include/Animation.hpp
	include/AudioManager.hpp
	include/ContentManager.hpp
	include/CustomMath.hpp
	include/Game.hpp
	include/GameState.hpp
	include/ServiceLocator.hpp
	include/Tileset.hpp
)

set(ENGINE_SOURCES
	src/AnimatedSprite.cpp
	src/Animation.cpp
	src/Game.cpp
	src/Tileset.cpp
)

add_library(gjt_engine STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})
target_include_directories(gjt_engine PUBLIC include)

find_package(SFML 2.5 COMPONENTS system graphics window audio REQUIRED)

if(SFML_FOUND)
	target_include_directories(gjt_engine PUBLIC ${SFML_DIR}/../../../include)
	target_link_libraries(gjt_engine sfml-system sfml-graphics sfml-window sfml-audio)
endif()

target_link_libraries(gjt_engine imgui)
