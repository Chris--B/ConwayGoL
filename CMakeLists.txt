cmake_minimum_required(VERSION 2.8.3)

project(ConwayGoL)

if(WIN32)
	# I've not gotten find_library to work on Windows. Possibly a setup issue.
	set(SFML_LIBS

		debug sfml-main-d.lib
		debug sfml-graphics-d.lib
		debug sfml-window-d.lib
		debug sfml-system-d.lib

		optimized sfml-main.lib
		optimized sfml-graphics.lib
		optimized sfml-window.lib
		optimized sfml-system.lib
	)

	set(GTEST_LIBS
		debug gtest_maind.lib
		debug gtestd.lib

		optimized gtest_main.lib
		optimized gtest.lib
	)

	add_definitions("/W3")
else()
	# TODO: Can XCode make use of more IDE-specific things?
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
	#     possible enum not being used when polling events.
	set(
		CMAKE_CXX_FLAGS
		"${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-switch"
		)
endif()

set(CONWAY_INCLUDES
	${CMAKE_SOURCE_DIR}/ConwayApp/include/board.h
	${CMAKE_SOURCE_DIR}/ConwayApp/include/cell.h
	${CMAKE_SOURCE_DIR}/ConwayApp/include/game.h
)
set(CONWAY_SOURCES
	${CMAKE_SOURCE_DIR}/ConwayApp/source/board.cpp
	${CMAKE_SOURCE_DIR}/ConwayApp/source/cell.cpp
	${CMAKE_SOURCE_DIR}/ConwayApp/source/game.cpp
)

include_directories(
	ConwayApp/include
	inih/include
)

add_subdirectory(inih)
add_subdirectory(Tests)

add_executable(
	Conway
	WIN32 MACOSX_BUNDLE

	ConwayApp/source/main.cpp
	${CONWAY_INCLUDES}
	${CONWAY_SOURCES}
)

target_link_libraries(
	Conway
	inihcpp
	${SFML_LIBS}
)
