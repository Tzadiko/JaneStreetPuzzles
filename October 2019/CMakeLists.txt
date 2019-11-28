cmake_minimum_required (VERSION 3.9 FATAL_ERROR)
project (JaneStreetPuzzle\ October\ 2019)
set (CMAKE_CXX_STANDARD 14)
find_package(Threads)
add_executable (mySolution TriTriAgainAgain.cpp)
set_target_properties (
	mySolution 
	PROPERTIES 
	OUTPUT_NAME "mySolution" 
		SUFFIX ".exe"
)

