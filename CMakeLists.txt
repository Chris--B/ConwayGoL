cmake_minimum_required(VERSION 2.8.3)

project(Conway CXX C)
OPTION(CONWAY_TESTS "Build tests" OFF)

if(${CONWAY_TEST})
	enable_testing()
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CLANG "IN_USE")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set(GCC "IN_USE")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
	set(INTEL "IN_USE")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	# MSVC is already auto-set.
else()
	message(SEND_ERROR
		"Your compiler is '${CMAKE_CXX_COMPILER_ID}',"
		" but I can't do anything with that. Sorry."
	)
endif()

# There are two major compilers we need to worry about: MSVC and GCC.
# Clang follows GCC's interface, and Intel MSVC's.
if(MSVC OR INTEL)
	# MSVC has C++11 support turned on by default.
	# Various standard headers use C++ exceptions. We don't have a compelling
	#     reason to disable C++ exceptions, so instead just enable
	#     unwind semantics and make the compiler happy.
	# MSVC's security warnings are valuable, but also somewhat of a nuisance.
	#     We disable them as an easy-fix and avoid using them in our code.
	set(CMAKE_CXX_FLAGS "/W4 /EHsc /D_CRT_SECURE_NO_WARNINGS")
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

	# -Wswitch complains about our event-handling switch statementS.
	# -Wmissing-braces complains about initializing std::array objects.
	set(DISABLED_WARNINGS "-Wno-switch -Wno-missing-braces")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra ${DISABLED_WARNINGS}")
else()
	message(SEND_ERROR
		"I can't figure out your compiler. Submit a bug report. "
		"Your Compiler ID is '${CMAKE_CXX_COMPILER_ID}'"
	)
endif()

# SFML
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/extern/SFML2")
subdirs(${SFML_ROOT})
include_directories(${SFML_ROOT}/include)

include_directories(include)

add_executable(
	Conway
	WIN32

	source/main.cpp

	source/Board.cpp
	source/Cell.cpp
	source/Game.cpp
	source/Settings.cpp
	source/ini.c
	source/inireader.cpp

	include/Board.hpp
	include/Cell.hpp
	include/Game.hpp
	include/Settings.hpp
	include/ini.h
	include/inireader.hpp
)

target_link_libraries(
	Conway

	# Will this work off of Windows?
	sfml-main

	sfml-audio
	sfml-graphics
	sfml-network
	sfml-system
	sfml-window
)

if (${CONWAY_TESTS})
	add_executable(
		ConwayTests

		test/gtest_all.cpp
		test/gtest_main.cpp

		test/CellTest.cpp
		source/Cell.cpp

		test/BoardTest.cpp
		source/Board.cpp

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
endif()
