// ****************************************************************************
// dynamo/parse/statement.hpp
//
// Statement parser for dynamo syntax. 
// Parses a string into a series of op_codes to be evaluated by the vm.
//
// Based on Boost.Spirit samples Copyright (c) 2001-2011 Joel de Guzman
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _DYNAMO_PARSER_STATEMENT_HPP_
#define _DYNAMO_PARSER_STATEMENT_HPP_
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

#include "dynamo/config.hpp"
#include "dynamo/ast/fusion_ast.hpp"
#include "dynamo/error_handler.hpp"
#include "dynamo/annotation.hpp"
#include "dynamo/parse/expression.hpp"
#include "dynamo/parse/qi.hpp"

#include <boost/spirit/include/phoenix_function.hpp>

namespace dynamo { namespace parse
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	///////////////////////////////////////////////////////////////////////////////
	//  The dynamo grammar
	///////////////////////////////////////////////////////////////////////////////
	template <typename Iterator>
	class statement : public qi::grammar<Iterator, ast::statement(), ascii::space_type>
	{
	public:

		statement(error_handler<Iterator>& error_handler_) 
			: statement::base_type(primary_statement)
			, expr(error_handler_)
		{
			// define terminals because of BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
			//qi::eol_type eol;
			qi::_1_type _1;
			qi::_3_type _3;
			qi::_4_type _4;
			qi::_val_type _val;

			namespace phx = boost::phoenix;
			typedef phx::function<error_handler<Iterator>> error_handler_function;
			typedef phx::function<annotation<Iterator>> annotation_function;

			using qi::on_error;
			using qi::on_success;
			using qi::fail;

			///////////////////////////////////////////////////////////////////////
			// Main statement grammar
			primary_statement =
				(
					(	assignment
					|	expr
					)
					> ';'
				) 
				| ';'
				;

			assignment =
					expr.identifier 
				>>	'='
				>	expr
				;

			// Debugging and error handling and reporting support.
			BOOST_SPIRIT_DEBUG_NODES(
				(primary_statement)
				(assignment)
			);

			///////////////////////////////////////////////////////////////////////
			// Error handling: on error in expr, call error_handler.
			on_error<fail>(primary_statement,
				error_handler_function(error_handler_)(
                "Error! Expecting ", _4, _3)
			);

			on_success(assignment,
				annotation_function(error_handler_.iters_)(_val, _1));
		}

		typedef ascii::space_type skp;
		
		qi::rule<Iterator, ast::statement(), skp> primary_statement;
		qi::rule<Iterator, ast::assignment(), skp> assignment;
		expression<Iterator> expr;
	};
}}

#endif // _DYNAMO_PARSER_STATEMENT_HPP_