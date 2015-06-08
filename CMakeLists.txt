cmake_minimum_required(VERSION 2.8)
project(dynamo)

###############################################################################
#
# Versioning
#
###############################################################################
set(DYNAMO_VERSION_MAJOR 1)
set(DYNAMO_VERSION_MINOR 0)
set(DYNAMO_VERSION_PATCH 0)
set(DYNAMO_VERSION_STRING ${DYNAMO_VERSION_MAJOR}.${DYNAMO_VERSION_MINOR}.${DYNAMO_VERSION_PATCH})
message("Dynamo Version: ${DYNAMO_VERSION_STRING}")
 
###############################################################################
#
# Options
#
###############################################################################
option( DYNAMO_BUILD_TESTS "Build the dynamo project tests." ON)
option( DYNAMO_BUILD_SAMPLES "Build the dynamo project samples." ON)

###############################################################################
#
# Find Boost
#
###############################################################################
set(Boost_USE_MULTI_THREADED ON)
set(Boost_USE_STATIC_LIBS ON)
find_package( Boost )

###############################################################################
#
# determine install path for CMake config files
#
###############################################################################
include(GNUInstallDirs)
if(WIN32 AND NOT CYGWIN)
	set(DEF_INSTALL_CMAKE_DIR CMake)
else()
	set(DEF_INSTALL_CMAKE_DIR lib/cmake/dynamo)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative cmake install path absolute (needed later on)
if(NOT IS_ABSOLUTE "${INSTALL_CMAKE_DIR}")
	set(INSTALL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_CMAKE_DIR}")
endif()

###############################################################################
#
# Configure CMake
#
###############################################################################
configure_file(
  "${PROJECT_SOURCE_DIR}/config/config.hpp.in"
  "${PROJECT_BINARY_DIR}/dynamo/config.hpp" )
 
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(Dynamo_INCLUDE_DIRS 
  ${PROJECT_SOURCE_DIR}/include
   ${PROJECT_BINARY_DIR}
   ${Boost_INCLUDE_DIR}
)

set(Dynamo_LIBRARIES dynamo)

if (CMAKE_COMPILER_IS_GNUCXX)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  if(COMPILER_SUPPORTS_CXX11)
  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has may not support Please use a different C++ compiler if building fails.")
  endif()
endif()

enable_testing()

add_subdirectory(src)
if(DYNAMO_BUILD_TESTS)
	add_subdirectory(test)
endif()

if(DYNAMO_BUILD_SAMPLES)
  add_subdirectory(sample)
endif()

###############################################################################
#
# Export Targets
#
# (so entity can be easily used by other CMake projects)
# [see http://www.cmake.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file]
#
###############################################################################

# Add all targets to the build-tree export set
export(TARGETS dynamo
	FILE "${PROJECT_BINARY_DIR}/dynamoTargets.cmake")
# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE dynamo)
# Create the dynamoConfig.cmake and dynamoConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
# ... for the build tree
configure_file(dynamoConfig.cmake.in
	"${PROJECT_BINARY_DIR}/dynamoConfig.cmake" @ONLY)
# ... for the install tree
configure_file(dynamoConfig.cmake.in
	"${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/dynamoConfig.cmake" @ONLY)
# ... for both
configure_file(dynamoConfigVersion.cmake.in
    "${PROJECT_BINARY_DIR}/dynamoConfigVersion.cmake" @ONLY)
# Install the dynamoConfig.cmake
install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/dynamoConfig.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}"
    COMPONENT dev)
# Install the export set for use with the install-tree
# install(EXPORT dynamoTargets
#     DESTINATION "${INSTALL_CMAKE_DIR}"
#     COMPONENT dev)