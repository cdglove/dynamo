cmake_minimum_required(VERSION 2.8)
project (dynamo)
 
file(GLOB_RECURSE  PROJECT_SOURCES *.cpp)
file(GLOB_RECURSE  PROJECT_HEADERS ../include/*.hpp)
 
find_package( Boost )

add_library( ${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ) 
target_include_directories( ${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR} ${Dynamo_INCLUDE_DIRS})


