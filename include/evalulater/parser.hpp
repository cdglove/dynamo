// ****************************************************************************
// evalulater/parser.hpp
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
#ifndef _EVALULATER_PARSER_HPP_
#define _EVALULATER_PARSER_HPP_
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Spirit v2.5 allows you to suppress automatic generation
// of predefined terminals to speed up complation. With
// BOOST_SPIRIT_NO_PREDEFINED_TERMINALS defined, you are
// responsible in creating instances of the terminals that
// you need (e.g. see qi::uint_type uint_ below).
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Define this to enable debugging
#define BOOST_SPIRIT_QI_DEBUG

#if defined(_MSC_VER)
# pragma warning(disable: 4345)
#endif

#include "evalulater/config.hpp"
#include "evalulater/ast.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

namespace evalulater
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	using boost::phoenix::function;

	///////////////////////////////////////////////////////////////////////////////
	//  The error handler - pretty prints a helful error when parsing fails
	///////////////////////////////////////////////////////////////////////////////
	struct error_handler_
	{
		template <typename T0 = void, typename T1 = void, typename T2 = void>
		struct result { typedef void type; };

		template <typename Iterator>
		void operator()(
			qi::info const& what
			, Iterator err_pos, Iterator last) const
		{
			std::cout
				<< "Error! Expecting "
				<< what                         // what failed?
				<< " here: \""
				<< std::string(err_pos, last)   // iterators to error-pos, end
				<< "\""
				<< std::endl
				;
		}
	};

	function<error_handler_> const error_handler = error_handler_();

	///////////////////////////////////////////////////////////////////////////////
	//  The evalulater grammar
	///////////////////////////////////////////////////////////////////////////////
	template <typename Iterator>
	struct parser : qi::grammar<Iterator, ast::expression(), ascii::space_type>
	{
		parser() : parser::base_type(expression)
		{
			qi::float_type float_;
			qi::_2_type _2;
			qi::_3_type _3;
			qi::_4_type _4;

			using qi::on_error;
			using qi::fail;

			///////////////////////////////////////////////////////////////////////
			// Tokens
			binary_tok.add
				("+", ast::bop_add)
				("-", ast::bop_subtract)
				("*", ast::bop_multiply)
				("/", ast::bop_divide)
				;

			unary_tok.add
				("+", ast::uop_positive)
				("-", ast::uop_negative)
				;

			intrinsic_tok.add
				("abs",	ast::iop_abs)		
				("pow",	ast::iop_pow)
				("add",	ast::iop_add)
				("sub",	ast::iop_subtract)
				("mul",	ast::iop_multiply)
				("div",	ast::iop_divide)
				;

			///////////////////////////////////////////////////////////////////////
			// Grammer
			
			expression =
                operand
                >> *operand
                ;

			operand =
				 intrinsic_op
				| binary_op
				| unary_op
				| float_
				| '(' > expression > ')'
				;

			unary_op = 
				unary_tok > operand
				;

			binary_op = 
				operand	> binary_tok > operand
				;

			intrinsic_op =
				intrinsic_tok >	'(' > *(operand % ',') > ')'
				;

			// Debugging and error handling and reporting support.
			BOOST_SPIRIT_DEBUG_NODES(
				(expression)(operand)(unary_op)(binary_op)(intrinsic_op));

			// Error handling
			on_error<fail>(expression, error_handler(_4, _3, _2));
		}

		qi::rule<Iterator, ast::expression(), ascii::space_type> expression;
		qi::rule<Iterator, ast::operand(), ascii::space_type> operand;
		qi::rule<Iterator, ast::unary_op(), ascii::space_type> unary_op;
		qi::rule<Iterator, ast::binary_op(), ascii::space_type> binary_op;
		qi::rule<Iterator, ast::intrinsic_op(), ascii::space_type> intrinsic_op;

		qi::symbols<char, ast::uop_token> unary_tok;
		qi::symbols<char, ast::bop_token> binary_tok;
		qi::symbols<char, ast::iop_token> intrinsic_tok;
	};
}

#endif // _EVALULATER_PARSER_HPP_