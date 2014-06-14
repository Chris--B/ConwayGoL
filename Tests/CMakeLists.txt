
include_directories(${CMAKE_SOURCE_DIR}/ConwayApp/include)

add_executable(
	ConwayTests

	BoardTests.cpp
	CellTests.cpp

	${CONWAY_INCLUDES}
	${CONWAY_SOURCES}
)

target_link_libraries(
	ConwayTests

	inihcpp
	${SFML_LIBS}
	${GTEST_LIBS}
)
