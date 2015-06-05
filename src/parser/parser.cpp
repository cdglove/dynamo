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

#if defined(_MSC_VER)
# pragma warning(disable: 4244)
#endif

#include "evalulater/parser/parser.hpp"
#include "evalulater/parser/statement.hpp"

namespace evalulater { namespace parser
{
	template<typename Iterator>
	static boost::optional<ast::statement_list> parse_impl(Iterator first, Iterator last)
	{
        evalulater::error_handler<						// Our diagnostic printer
			Iterator
		> error_handler(first, last);						

		evalulater::parser::statement<					// Our grammar
			Iterator
		> stmt(error_handler);

		boost::spirit::ascii::space_type space;
		evalulater::ast::statement_list ast;
		bool r = phrase_parse(first, last, +stmt, space, ast);
		if(r && first == last)
		{
			return ast;
		}

		return boost::none;
	}

	boost::optional<ast::statement_list> parser::parse(std::string const& text)
	{
		return parse_impl(text.begin(), text.end());
	}

	boost::optional<ast::statement_list> parser::parse(std::wstring const& text)
	{
		return parse_impl(text.begin(), text.end());
	}

	boost::optional<ast::statement_list> parser::parse(char const* first, char const* last)
	{
		return parse_impl(first, last);
	}

	boost::optional<ast::statement_list> parser::parse(wchar_t const* first, wchar_t const* last)
	{
		return parse_impl(first, last);
	}
}}