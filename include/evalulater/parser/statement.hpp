// ****************************************************************************
// evalulater/parser/statement.hpp
//
// Parser for evalulater syntax.  Based on Boost.Spirit calc6 example
// Parses a string into a series of op_codes to be evaluated by the vm.
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _EVALULATER_PARSER_STATEMENT_HPP_
#define _EVALULATER_PARSER_STATEMENT_HPP_
#pragma once

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
# pragma warning(disable: 4345)
#endif

#include "evalulater/config.hpp"
#include "evalulater/ast/fusion_ast.hpp"
#include "evalulater/error_handler.hpp"
#include "evalulater/parser/expression.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

namespace evalulater { namespace parser
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	///////////////////////////////////////////////////////////////////////////////
	//  The evalulater grammar
	///////////////////////////////////////////////////////////////////////////////
	template <typename Iterator>
	class statement : public qi::grammar<Iterator, ast::statement(), ascii::space_type>
	{
	public:

		statement(error_handler<Iterator>& error_handler_) 
			: statement::base_type(statement_)
			, expr(error_handler_)
		{
			// define terminals because of BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
			//qi::eol_type eol;
			qi::_3_type _3;
			qi::_4_type _4;

			namespace phx = boost::phoenix;
			typedef phx::function<error_handler<Iterator>> error_handler_function;

			using qi::on_error;
			using qi::fail;

			///////////////////////////////////////////////////////////////////////
			// Main statement grammar
			statement_ =
				(	assignment
				|	expr
				)
				> ';'
				;

			assignment =
					expr.identifier 
				>>	'='
				>	expr
				;

			// Debugging and error handling and reporting support.
			BOOST_SPIRIT_DEBUG_NODES(
				(statement_)
				(assignment)
			);

			///////////////////////////////////////////////////////////////////////
			// Error handling: on error in expr, call error_handler.
			on_error<fail>(statement_,
				error_handler_function(error_handler_)(
                "Error! Expecting ", _4, _3)
			);
		}

		typedef ascii::space_type skp;
		
		qi::rule<Iterator, ast::statement(), skp> statement_;
		qi::rule<Iterator, ast::assignment(), skp> assignment;
		expression<Iterator> expr;
	};
}}

#endif // _EVALULATER_PARSER_STATEMENT_HPP_