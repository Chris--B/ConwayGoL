cmake_minimum_required(VERSION 2.8.3)

project(ConwayGoL)

set(GCC_CLANG_FLAGS "-std=c++11 -Wall -Wextra -Wno-switch -Wno-missing-braces")

if(WIN32)
	add_definitions(-DSFML_STATIC)

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
	set(SFML_LIBS ${SFML_LIBS_DEBUG} ${SFML_LIBS_RELEASE})
else()
	if(APPLE)
		# OS X needs this (why?), but GCC can't handle it.
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
		# Xcode (but not the makefiles) ignores that last line, so we need to hardcode it.
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	endif()

	find_library(SFML_GRAPHICS sfml-graphics)
	find_library(SFML_SYSTEM sfml-system)
	find_library(SFML_WINDOW sfml-window)
	set(SFML_LIBS ${SFML_GRAPHICS} ${SFML_SYSTEM} ${SFML_WINDOW})
endif()

if(MSVC)
	# Nothing, I just can't find a `not` command
else()
	# If you're not using MSVC, you're using Clang or GCC... or Intel. Let's ignore Intel for now.
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CLANG_FLAGS}")
endif()

set(srcs
	source/board.cpp
	source/cell.cpp
	source/game.cpp
	source/main.cpp
)

set(incs
	include/board.h
	include/cell.h
	include/game.h
	)
include_directories(include)

add_executable(
	Conway
	WIN32 MACOSX_BUNDLE
	${srcs}
	${incs}
)
target_link_libraries(
	Conway
	${SFML_LIBS})
