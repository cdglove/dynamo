// Generated from config.hpp.in by CMake 3.0.1
// Manually added here for build systems that dont
// use CMake.

#ifndef DYNAMO_CONFIG_H
#define DYNAMO_CONFIG_H

const int DynamoVersionMajor = 1;
const int DynamoVersionMinor = 0;
const int DynamoVersionPatch = 0;
char const* const DynamoVersionString = "1.0.0";


///////////////////////////////////////////////////////////////////////////////
// Spirit v2.5 allows you to suppress automatic generation
// of predefined terminals to speed up compilation. With
// BOOST_SPIRIT_NO_PREDEFINED_TERMINALS defined, you are
// responsible in creating instances of the terminals that
// you need (e.g. see qi::uint_type uint_ below).
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Define this to enable debugging
//#define BOOST_SPIRIT_QI_DEBUG

#if defined(_MSC_VER)
# pragma warning(disable: 4345 4714)
#endif

#include <boost/config.hpp>
#include <boost/config/warning_disable.hpp>

#endif
