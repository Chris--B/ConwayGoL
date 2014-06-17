cmake_minimum_required(VERSION 2.8.3)

project(Conway CXX C)
enable_testing()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CLANG "IN_USE")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(GCC "IN_USE")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
	set(INTEL "IN_USE")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# MSVC is already auto-set.
else()
	message(SEND_ERROR
		"Your compiler is '${CMAKE_CXX_COMPILER_ID}',"
		" but I can't do anything with that. Submit a bug report."
	)
endif()

# Setup / find SFML libs.
if(WIN32)
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
else()
	find_library(SFML_GRAPHICS sfml-graphics)
	find_library(SFML_SYSTEM sfml-system)
	find_library(SFML_WINDOW sfml-window)
	set(SFML_LIBS ${SFML_GRAPHICS} ${SFML_SYSTEM} ${SFML_WINDOW})
endif()

# There are two major compilers we need to worry about: MSVC and GCC.
# Clang follows GCC's interface, and Intel MSVC's.
if(MSVC OR INTEL)
	# MSVC has C++11 support turned on by default.
	set(CMAKE_CXX_FLAGS "/W4")
elseif(GCC OR CLANG)
	# Older versions of GCC and Clang expect -std=c++0x.
	# We are not supporting those versions at this time.
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

	# When compiling C++11 on OS X, we need to use libc++ instead.
	# I'm not sure why, but it's what StackOverflow suggets and it works.
	if(APPLE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
	endif()

	# When compiling C++11 on OS X, we need to use libc++ instead.
	# I'm not sure why, but it's what StackOverflow suggets and it works.
	if(APPLE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
	endif()

	# -Wswitch complains about our event-handling switch statements,
	#     so we disable it here.
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-switch")
else()
	message(SEND_ERROR
		"I can't figure out your compiler. Submit a bug report. "
		"Your Compiler ID is '${CMAKE_CXX_COMPILER_ID}'"
	)
endif()

include_directories(include)

add_executable(
	Conway
	WIN32 MACOSX_BUNDLE

	source/main.cpp

	source/board.cpp
	source/cell.cpp
	source/game.cpp
	source/ini.c
	source/inireader.cpp

	include/board.h
	include/cell.h
	include/game.h
	include/ini.h
	include/inireader.h
)

target_link_libraries(
	Conway
	${SFML_LIBS}
)

add_executable(
	ConwayTests

	test/gtest_all.cpp
	test/gtest_main.cpp

	test/CellTest.cpp
	source/cell.cpp

	test/BoardTest.cpp
	source/board.cpp

	include/gtest/gtest.h
)
# Windows doesn't need to link against a threading library for gtest.
if(NOT WIN32)
	target_link_libraries(
		ConwayTests
		pthread
	)
endif()
add_test(gtest ConwayTests)
