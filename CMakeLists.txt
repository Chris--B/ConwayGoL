cmake_minimum_required(VERSION 2.8.3)

project(ConwayGoL)
add_subdirectory(inih)

if(WIN32)
	# I've not gotten find_library to work on Windows. Likely a setup issue.
	set(SFML_LIBS_DEBUG
		debug sfml-main-d.lib
		debug sfml-graphics-d.lib
		debug sfml-window-d.lib
		debug sfml-system-d.lib
	)
	set(SFML_LIBS_RELEASE
		optimized sfml-main.lib
		optimized sfml-graphics.lib
		optimized sfml-window.libf
		optimized sfml-system.lib
	)
	set(SFML_LIBS ${SFML_LIBS_DEBUG} ${SFML_LIBS_RELEASE})
	add_definitions("/W3")
else()
	# TODO: More IDE-specific things for XCode?
	if(APPLE)
		# OS X needs this (why?), but GCC can't handle it.
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
		# Xcode (but not the makefiles) ignores that last line,
		#     so we need to hardcode it.
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	endif()

	find_library(
		SFML_LIBS
		sfml-graphics
		sfml-system
		sfml-window
	)
endif()

if(NOT MSVC)
	# If you're not using MSVC, you're using Clang or GCC... or Intel.
	#     Let's ignore Intel for now.
	# Older compilers don't support -std=c++11, but do support
	#     -std=c++0x. We don't care about them.
	# We disable -Wswitch so the compiler doesn't warn about every
	#     possible enum when polling events.
	set(
		CMAKE_CXX_FLAGS
		"${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-switch"
		)
endif()

include_directories(include)
include_directories(inih/include)

add_executable(
	Conway
	WIN32 MACOSX_BUNDLE

	source/board.cpp
	source/cell.cpp
	source/game.cpp
	source/main.cpp

	include/board.h
	include/cell.h
	include/game.h
)

target_link_libraries(
	Conway
	inihcpp
	${SFML_LIBS}
)
