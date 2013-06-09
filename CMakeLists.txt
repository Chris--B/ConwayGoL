cmake_minimum_required(VERSION 2.8.3)

project(ConwayGoL)

set(SFML_LIBS_DEBUG
	debug sfml-main-d.lib
	debug sfml-graphics-s-d.lib
	debug sfml-window-s-d.lib
	debug sfml-system-s-d.lib
)
set(SFML_LIBS_RELEASE
	optimized sfml-main.lib
	optimized sfml-graphics-s.lib
	optimized sfml-window-s.lib
	optimized sfml-system-s.lib
)

add_definitions(-DSFML_STATIC)

# Visual Studio ignores this, but anything mimicing GCC's interface will need it, or possibly '-std=c++0x'
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(src "source")
set(sources
	${src}/board.cpp
	${src}/cell.cpp
	${src}/game.cpp
	${src}/main.cpp
)

set(inc "include")
set(includes
	${inc}/board.h
	${inc}/cell.h
	${inc}/game.h
	)
include_directories(${inc})

add_executable(
	Conway
	WIN32 MACOSX_BUNDLE
	${sources}
	${includes}
)
target_link_libraries(
	Conway
	${SFML_LIBS_DEBUG}
	${SFML_LIBS_RELEASE})
