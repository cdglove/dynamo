cmake_minimum_required(VERSION 2.8)
project (dynamo_test)
 
set (PROJECT_INCLUDE_DIR "../")
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
 
find_package( Boost )

# -----------------------------------------------------------------------------
# Function to add a normal sample with all of the proper includes, etc.
#------------------------------------------------------------------------------
function(create_and_add_sample sample_name sample_source)
	add_executable(sample.${sample_name} ${sample_source})
	target_link_libraries(sample.${sample_name} ${Dynamo_LIBRARIES})
	target_include_directories(sample.${sample_name} PUBLIC ${Boost_INCLUDE_DIR} ${Dynamo_INCLUDE_DIRS})
endfunction(create_and_add_sample)

create_and_add_sample(repl repl.cpp)



