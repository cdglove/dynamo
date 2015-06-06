// ****************************************************************************
// evalulater/parser/parser.hpp
//
// Firewall object to shield code from dragging in boost.spirit
// Implements the parsing interface for common types, but of course users
// can bypass this object and call phrase_pase directly.
// 
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _EVALULATER_PARSER_PARSER_HPP_
#define _EVALULATER_PARSER_PARSER_HPP_
#pragma once

#include <string>
#include <boost/optional.hpp>
#include "evalulater/ast/ast.hpp"

namespace evalulater
{
	class diagnostic_sink;
}

namespace evalulater { namespace parser
{
	class parser
	{
	public:

		parser(diagnostic_sink& sink)
			: diagnostic(sink)
		{}

		boost::optional<ast::statement_list> parse(std::string const& text);
		boost::optional<ast::statement_list> parse(std::wstring const& text);
		boost::optional<ast::statement_list> parse(char const* first, char const* last);
		boost::optional<ast::statement_list> parse(wchar_t const* first, wchar_t const* last);

	private:

		diagnostic_sink& diagnostic;
	};
}}

#endif // _EVALULATER_PARSER_PARSER_HPP_