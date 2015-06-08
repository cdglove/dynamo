// ****************************************************************************
// dynamo/parse/expression.hpp
//
// Parser for dynamo syntax. 
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
#ifndef _DYNAMO_PARSER_EXPRESSION_HPP_
#define _DYNAMO_PARSER_EXPRESSION_HPP_
#pragma once

#include "dynamo/config.hpp"
#include "dynamo/ast/fusion_ast.hpp"
#include "dynamo/error_handler.hpp"
#include "dynamo/annotation.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

namespace dynamo { namespace parse
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	///////////////////////////////////////////////////////////////////////////////
	//  The dynamo grammar
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

			qi::_3_type _1;
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
				("not",	ast::op_not)
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

			on_success(intrinsic_op,
				annotation_function(error_handler_.iters)(_val, _1)
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

#endif // _DYNAMO_PARSER_EXPRESSION_HPP_