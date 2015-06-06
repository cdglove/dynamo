cmake_minimum_required(VERSION 2.8)
project (evalulater_test)
 
set (PROJECT_INCLUDE_DIR "../")
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
 
set(Boost_USE_MULTI_THREADED ON)
set(Boost_USE_STATIC_LIBS ON)
find_package( Boost
	REQUIRED
    unit_test_framework
)

# -----------------------------------------------------------------------------
# Function to add an include test with all of the proper paths etc.
#------------------------------------------------------------------------------
function(add_include_test header_path header_name)
	if(header_path)
		set(test_name test.include.${header_path}.${header_name})
	else()
		set(test_name test.include.${header_name})
	endif()

	add_executable(${test_name} include.cpp)
	target_include_directories(${test_name} PUBLIC ${Evalulater_INCLUDE_DIRS})
	add_test(${test_name} ${test_name})
	set_property(TARGET ${test_name} APPEND PROPERTY COMPILE_DEFINITIONS "HEADER=\"evalulater/${header_path}/${header_name}.hpp\"")
endfunction(add_include_test)

add_include_test("" compiler)
add_include_test("" error_handler)
add_include_test(ast ast)
add_include_test(ast fusion_ast)
add_include_test(vm machine)
add_include_test(vm byte_code)
add_include_test(vm executable)
add_include_test(parser parser)
add_include_test(parser expression)
add_include_test(parser statement)

# -----------------------------------------------------------------------------
# Function to add a normal test with all of the proper includes, etc.
#------------------------------------------------------------------------------
function(create_and_add_test test_name test_source)
	add_executable(test.${test_name} ${test_source})
	target_link_libraries(test.${test_name} ${Boost_LIBRARIES} ${Evalulater_LIBRARIES})
	target_include_directories(test.${test_name} PUBLIC ${Boost_INCLUDE_DIR} ${Evalulater_INCLUDE_DIRS})
	add_test(test.${test_name} test.${test_name})
endfunction(create_and_add_test)

create_and_add_test(parser parser.cpp)
create_and_add_test(operators operators.cpp)
