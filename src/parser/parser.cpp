// ****************************************************************************
// evalulater/parser/parser.cpp
//
// Implementation of parser firewall object.
// 
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************

#include "evalulater/parser/parser.hpp"

// Inclue at least one instantiation of the parser in the lib.
template class evalulater::parse::parser<std::string::const_iterator>;
