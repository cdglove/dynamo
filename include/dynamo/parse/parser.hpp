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
#ifndef _DYNAMO_PARSE_PARSER_HPP_
#define _DYNAMO_PARSE_PARSER_HPP_
#pragma once

#include "dynamo/config.hpp"
#include "dynamo/ast/ast.hpp"
#include "dynamo/error_handler.hpp"
#include <string>
#include <boost/optional.hpp>

namespace dynamo { namespace parse
{
	///////////////////////////////////////////////////////////////////////////
	// The Parser
	// Consumes a sequence of characters and returns an ast on success
	// or boost::none on failure.
	///////////////////////////////////////////////////////////////////////////
	class parser
	{
	public:

		parser(diagnostic_sink& diagnostic)
			: diagnostic_(diagnostic)
		{}

		boost::optional<ast::statement_list> parse(std::string const& s) const;
		boost::optional<ast::statement_list> parse(char const* first, char const* last) const;

		template<typename Iterator>
		boost::optional<ast::statement_list> parse(
			Iterator first, 
			Iterator last) const
		{
			error_handler<Iterator> err_handler(diagnostic_, first, last);
			return parse(err_handler, first, last);
		}
		
		// The parser definition is intentionally excluded to reduce compile
		// times for those who only need the const char* version.
		// If you're getting a linker error because of this, include 
		// parser_def.hpp
		template<typename Iterator>
		static boost::optional<ast::statement_list> parse(
			error_handler<Iterator>& err_handler,
			Iterator first, 
			Iterator last
		);

	private:

		diagnostic_sink& diagnostic_;
	};
}}

#endif // _DYNAMO_PARSE_PARSER_HPP_