// ****************************************************************************
// evalulater/parser/parser.hpp
//
// Parser object for evalulater syntax.
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
#include <boost/any.hpp>
#include "evalulater/ast/ast.hpp"
#include "evalulater/parser/statement.hpp"

namespace evalulater { namespace parse
{
	///////////////////////////////////////////////////////////////////////////
	// The Parser
	// Consumes a sequence of characters and returns an ast on success
	// or boost::none on failure.
	///////////////////////////////////////////////////////////////////////////
	class parser
	{
	public:

		template<typename Iterator>
		parser(error_handler<Iterator>& error_handler_)
			: error_handler_(&error_handler_)
		{}

		boost::optional<ast::statement_list> parse(std::string const& s) const;

		template<typename Iterator>
		boost::optional<ast::statement_list> parse(
			Iterator first, 
			Iterator last) const
		{
			error_handler<Iterator>& err_handler = *boost::any_cast<
				error_handler<Iterator>*
			>(error_handler_);
			return parse(err_handler, first, last);
		}
		
		template<typename Iterator>
		static boost::optional<ast::statement_list> parse(
			error_handler<Iterator>& error_handler_,
			Iterator first, 
			Iterator last)
		{
			error_handler_.on_parse_begin(first, last);

			evalulater::parse::statement<	
				Iterator
			> stmt(error_handler_);

			boost::spirit::ascii::space_type space;
			evalulater::ast::statement_list ast;
			bool r = phrase_parse(first, last, +stmt, space, ast);
			if(r && first == last)
			{
				return ast;
			}

			return boost::none;
		}

	private:

		boost::any error_handler_;

	};
}}

#endif // _EVALULATER_PARSER_PARSER_HPP_