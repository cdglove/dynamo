cmake_minimum_required(VERSION 2.8)
project (evalulater_test)
 
set (PROJECT_INCLUDE_DIR "../")
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
 
find_package( Boost )

# -----------------------------------------------------------------------------
# Function to add an include test with all of the proper paths etc.
#------------------------------------------------------------------------------
function(add_include_test header_path header_name)
	add_executable(test.include.${header_path}.${header_name} include.cpp)
	target_include_directories(test.include.${header_path}.${header_name} PUBLIC ${Evalulater_INCLUDE_DIRS})
	add_test(test.include.${header_path}.${header_name} test.include.${header_name})
	set_property(TARGET test.include.${header_path}.${header_name}  APPEND PROPERTY COMPILE_DEFINITIONS "HEADER=\"evalulater/${header_path}/${header_name}.hpp\"")
endfunction(add_include_test)

add_include_test("" compiler)
add_include_test("" ast)
add_include_test("" error_handler)
add_include_test(vm machine)
add_include_test(vm byte_code)
add_include_test(parser expression)
add_include_test(parser statement)

# -----------------------------------------------------------------------------
# Function to add a normal test with all of the proper includes, etc.
#------------------------------------------------------------------------------
function(create_and_add_test test_name test_source)
	add_executable(test.${test_name} ${test_source})
	target_link_libraries(test.${test_name} ${Evalulater_LIBRARIES})
	target_include_directories(test.${test_name} PUBLIC ${Boost_INCLUDE_DIR} ${Evalulater_INCLUDE_DIRS})
	add_test(test.${test_name} test.${test_name})
endfunction(create_and_add_test)

create_and_add_test(example example.cpp)




