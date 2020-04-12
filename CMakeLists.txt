cmake_minimum_required(VERSION 3.0)

project(Conway LANGUAGES CXX C)
enable_testing()

find_package(SFML 2.5 COMPONENTS graphics audio window REQUIRED)

add_executable(Conway
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

target_include_directories(Conway
	PRIVATE
		include
)

target_link_libraries(Conway
	PRIVATE
		sfml-graphics
		sfml-system
		sfml-window
)

add_executable(ConwayTests
	test/gtest_all.cpp
	test/gtest_main.cpp

	test/CellTest.cpp

	test/BoardTest.cpp

	include/gtest/gtest.h

	include/Cell.hpp
	include/Board.hpp

	source/Cell.cpp
	source/Board.cpp
)

target_include_directories(ConwayTests
	PRIVATE
		include
)

# Windows doesn't need to link against a threading library for gtest.
if(NOT WIN32)
	target_link_libraries(
		ConwayTests
		pthread
	)
endif()
add_test(gtest ConwayTests)

function(apply_common_settings target)
	set_target_properties(${target}
		PROPERTIES
			CXX_STANDARD          14
			CXX_STANDARD_REQUIRED YES
			CXX_EXTENSIONS        NO
	)

	if(${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC|INTEL")
		target_compile_definitions(${target}
			PRIVATE
				_CRT_SECURE_NO_WARNINGS
		)
	else()
		target_compile_options(${target}
			PRIVATE
				-Wall
				-Wextra
				# -Wswitch complains about our event-handling switch statementS.
				-Wno-switch
				# -Wmissing-braces complains about initializing std::array objects.
				-Wno-missing-braces
		)
	endif()
endfunction()

apply_common_settings(Conway)
apply_common_settings(ConwayTests)
