cmake_minimum_required(VERSION 2.8)
project (evalulater)
 
file(GLOB_RECURSE  PROJECT_SOURCES *.cpp)
file(GLOB_RECURSE  PROJECT_HEADERS ../include/*.hpp)
 
find_package( Boost )

add_library( ${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ) 
target_include_directories( ${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR} ${Evalulater_INCLUDE_DIRS})


