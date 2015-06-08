// ****************************************************************************
// dynamo/parser/parser.cpp
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

#include "dynamo/parser/parser.hpp"

namespace dynamo { namespace parse
{
	// Include at least one instantiation of the parser in the lib.
	boost::optional<
		ast::statement_list
	> parser::parse(std::string const& s) const
	{
		return parse(s.begin(), s.end());
	}
}}