cmake_minimum_required(VERSION 2.8)
project(evalulater)

###############################################################################
#
# Versioning
#
###############################################################################
set(EVALULATER_VERSION_MAJOR 1)
set(EVALULATER_VERSION_MINOR 0)
set(EVALULATER_VERSION_PATCH 0)
set(EVALULATER_VERSION_STRING ${EVALULATER_VERSION_MAJOR}.${EVALULATER_VERSION_MINOR}.${EVALULATER_VERSION_PATCH})
message("Evalulater Version: ${EVALULATER_VERSION_STRING}")
 
###############################################################################
#
# Options
#
###############################################################################
option( EVALULATER_BUILD_TESTS "Build the evalulater project tests." ON)

###############################################################################
#
# Find Boost
#
###############################################################################
set(Boost_USE_MULTI_THREADED ON)
set(Boost_USE_STATIC_LIBS ON)
find_package( Boost
  REQUIRED
    system
)

###############################################################################
#
# determine install path for CMake config files
#
###############################################################################
include(GNUInstallDirs)
if(WIN32 AND NOT CYGWIN)
	set(DEF_INSTALL_CMAKE_DIR CMake)
else()
	set(DEF_INSTALL_CMAKE_DIR lib/cmake/evalulater)
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
  "${PROJECT_BINARY_DIR}/evalulater/config.hpp" )
 
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EVALULATER_INCLUDE_DIRS 
  ${PROJECT_SOURCE_DIR}/include
   ${PROJECT_BINARY_DIR}
   ${Boost_INCLUDE_DIR}
)

#set(EVALULATER_LIBRARIES evalulater)

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

#add_subdirectory(src)
if(EVALULATER_BUILD_TESTS)
	add_subdirectory(test)
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
# export(TARGETS evalulater
# 	FILE "${PROJECT_BINARY_DIR}/evalulaterTargets.cmake")
# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE evalulater)
# Create the evalulaterConfig.cmake and evalulaterConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
# ... for the build tree
configure_file(evalulaterConfig.cmake.in
	"${PROJECT_BINARY_DIR}/evalulaterConfig.cmake" @ONLY)
# ... for the install tree
configure_file(evalulaterConfig.cmake.in
	"${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/evalulaterConfig.cmake" @ONLY)
# ... for both
configure_file(evalulaterConfigVersion.cmake.in
    "${PROJECT_BINARY_DIR}/evalulaterConfigVersion.cmake" @ONLY)
# Install the evalulaterConfig.cmake
install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/evalulaterConfig.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}"
    COMPONENT dev)
# Install the export set for use with the install-tree
# install(EXPORT evalulaterTargets
#     DESTINATION "${INSTALL_CMAKE_DIR}"
#     COMPONENT dev)