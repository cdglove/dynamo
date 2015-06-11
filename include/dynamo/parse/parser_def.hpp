// ****************************************************************************
// dynamo/parse/parser.hpp
//
// Parser object for dynamo syntax.
// 
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _DYNAMO_PARSE_PARSERDEF_HPP_
#define _DYNAMO_PARSE_PARSERDEF_HPP_
#pragma once

#include "dynamo/parse/parser.hpp"
#include "dynamo/parse/statement.hpp"

namespace dynamo { namespace parse
{
	template<typename Iterator>
	boost::optional<ast::statement_list> parser::parse(
		error_handler<Iterator>& err_handler,
		Iterator first, 
		Iterator last)
	{
		err_handler.on_parse_begin(first, last);

		dynamo::parse::statement<	
			Iterator
		> stmt(err_handler);

		boost::spirit::ascii::space_type space;
		dynamo::ast::statement_list ast;
		bool r = phrase_parse(first, last, +stmt, space, ast);
		if(r && first == last)
		{
			return ast;
		}

		return boost::none;
	}
}}

#endif // _DYNAMO_PARSE_PARSER_HPP_