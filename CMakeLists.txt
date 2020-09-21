cmake_minimum_required(VERSION 2.8)
project(RaceTest)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(ReceTest RaceTest.cpp Vector.cpp Vector.h)	
								