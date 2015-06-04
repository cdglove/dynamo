// ****************************************************************************
// evalulater/parser/expression.hpp
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
#ifndef _EVALULATER_PARSER_EXPRESSION_HPP_
#define _EVALULATER_PARSER_EXPRESSION_HPP_
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
#include "evalulater/ast.hpp"
#include "evalulater/error_handler.hpp"

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
	class expression : public qi::grammar<Iterator, ast::expression(), ascii::space_type>
	{
	public:

		expression(error_handler<Iterator>& error_handler_) 
			: expression::base_type(expr)
		{
			// define terminals because of BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
			qi::float_type float_;
			qi::raw_type raw;
	        qi::lexeme_type lexeme;
	        qi::alpha_type alpha;
	        qi::alnum_type alnum;

			qi::_3_type _3;
			qi::_4_type _4;

			namespace phx = boost::phoenix;
			typedef phx::function<error_handler<Iterator>> error_handler_function;

			using qi::on_error;
			using qi::fail;

			///////////////////////////////////////////////////////////////////////
			// Tokens
			binary_tok.add
				("+", ast::op_add)
				("-", ast::op_subtract)
				("*", ast::op_multiply)
				("/", ast::op_divide)
			;

			unary_tok.add
				("+", ast::op_positive)
				("-", ast::op_negative)
				("!", ast::op_not)
			;

			intrinsic_tok.add
				("abs",	ast::op_abs)		
				("pow",	ast::op_pow)
				("add",	ast::op_add)
				("sub",	ast::op_subtract)
				("mul",	ast::op_multiply)
				("div",	ast::op_divide)
			;

			///////////////////////////////////////////////////////////////////////
			// Main expression grammar
			expr =
					unary_expr
				>> *binary_op
				;

			unary_expr =
					primary_expr
				|   unary_op
				;

			primary_expr =
					float_
				|   intrinsic_op
				|   identifier
				|   '(' > expr > ')'
				;

			unary_op =
					unary_tok > unary_expr
				;

			binary_op = 
					binary_tok >> unary_expr
				;

			intrinsic_op =
					(intrinsic_tok >> '(')
				>   argument_list
				>   ')'
				;

			argument_list = -(expr % ',');

			identifier =
					!lexeme[intrinsic_tok >> !(alnum | '_')]
				>>  raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
				;

			// Debugging and error handling and reporting support.
			BOOST_SPIRIT_DEBUG_NODES(
				(expr)
				(primary_expr)
				(unary_expr)
				(unary_op)
				(binary_op)
				(intrinsic_op)
				(identifier)
			);

			///////////////////////////////////////////////////////////////////////
			// Error handling: on error in expr, call error_handler.
			on_error<fail>(expr,
				error_handler_function(error_handler_)(
                "Error! Expecting ", _4, _3)
			);
		}

		typedef ascii::space_type skp;
		
		qi::rule<Iterator, ast::expression(), skp> expr;
		qi::rule<Iterator, ast::operand(), skp> primary_expr;
		qi::rule<Iterator, ast::operand(), skp> unary_expr;
		qi::rule<Iterator, ast::unary_op(), skp> unary_op;
		qi::rule<Iterator, ast::binary_op(), skp> binary_op;
		qi::rule<Iterator, ast::intrinsic_op(), skp> intrinsic_op;
		qi::rule<Iterator, ast::identifier(), skp> identifier;

		qi::rule<Iterator, std::vector<ast::expression>(), skp> argument_list;
		
		qi::symbols<char, ast::op_token> binary_tok;
		qi::symbols<char, ast::op_token> unary_tok;
		qi::symbols<char, ast::op_token> intrinsic_tok;
	};
}}

#endif // _EVALULATER_PARSER_EXPRESSION_HPP_