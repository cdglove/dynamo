// ****************************************************************************
// dynamo/parse/parser.cpp
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

#if defined(_MSC_VER)
# pragma warning(disable: 6326 6244 4503)
#endif

#include "dynamo/parse/parser.hpp"
#include "dynamo/parse/parser_def.hpp"
#include "dynamo/diagnostic/source_index.hpp"

namespace dynamo { namespace parse
{
	// Include at least one instantiation of the parser in the lib.
	boost::optional<
		ast::statement_list
	> string_parser::parse(std::string src)
	{
		src.swap(src_);

		// Reuse the generic parser implementation.
		parser<std::string::const_iterator> parser(sink_);
		std::string const& c_src = src_;
		boost::optional<ast::statement_list> ret_val = parser.parse(
			c_src.begin(),
			c_src.end()
		);

		indexed_source_.identifier_iterators_.swap(
			parser.indexed_source_.identifier_iterators_
		);

		indexed_source_.first_ = parser.indexed_source_.first_;
		indexed_source_.last_ = parser.indexed_source_.last_;

		return ret_val;
	}	
}}

