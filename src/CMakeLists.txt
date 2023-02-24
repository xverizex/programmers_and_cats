set (CMAKE_CXX_COMPILER gcc)

include (FindPkgConfig)

if (DEFINED WEBGL)
else()
pkg_check_modules (SDL REQUIRED sdl2)
pkg_check_modules (GLES REQUIRED glesv2)
pkg_check_modules (FREETYPE REQUIRED freetype2)
endif()

add_executable (office
	core/game.c
	core/timer.c
	core/downloader.c
	core/shaders.c
	core/cmath.c
	core/sprite.c
	core/animation.c
	core/rect.c
	core/text.c
	shaders/2d_simple.c
	shaders/2d_simple_color.c
	levels/logo.c
	levels/office_0x0.c
	common/file.c
	common/bites.c
	common/coord.c
	game/human.c
	game/floor.c
	game/wall.c
	game/path.c
	game/board.c
	game/a_star.c
	game/fired.c
	game/productivity.c
	game/game_over.c
	main.c
	)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "-s USE_SDL=2 -DWEBGL -s USE_FREETYPE=1 -O2 -s WASM=1 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 -s FULL_ES3=1 --preload-file assets")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()

if (DEFINED WEBGL)
target_include_directories (office
	PRIVATE
	"include"
	"-I/usr/include/SDL2 -D_REENTRANT"
	"-IGLES3"
	"-IFreetype"
	)
else()
target_include_directories (office
	PRIVATE
	"include"
	${SDL_INCLUDE_DIRS}
	${GLES_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
	)
endif()

if (DEFINED WEBGL)
target_link_libraries (office
	PRIVATE
	"-s USE_SDL=2"
	"-lfreetype"
	"-lm"
	)

else()
target_link_libraries (office
	PRIVATE
	${SDL_LIBRARIES}
	${GLES_LIBRARIES}
	${FREETYPE_LIBRARIES}
	"-lm"
	)

endif()

set(EXECUTABLE_OUTPUT_PATH ..)