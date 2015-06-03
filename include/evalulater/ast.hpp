// ****************************************************************************
// evalulater/ast.hpp
//
// Abstract syntax tree for evalulater syntax.  Based on Boost.Spirit calc6 
// example.
// 
// Defines expressions and the data asociated with them.
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _EVALULATER_AST_HPP_
#define _EVALULATER_AST_HPP_
#pragma once

#include "evalulater/config.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <iostream>
#include <vector>

namespace evalulater { namespace ast
{
	///////////////////////////////////////////////////////////////////////////
	//  The AST
	///////////////////////////////////////////////////////////////////////////
	struct nil {};
	struct unary_op;
	struct binary_op;
	struct intrinsic_op;
	struct expression;

	struct identifier : std::string
	{
		identifier(std::string const& name = "") 
			: std::string(name) 
		{}

		template<typename Iterator>
		identifier(Iterator first, Iterator last) 
			: std::string(first, last) 
		{}
	};

	typedef boost::variant<
		  nil
		, float
		, identifier
		, boost::recursive_wrapper<unary_op>
		, boost::recursive_wrapper<binary_op>
		, boost::recursive_wrapper<intrinsic_op>
		, boost::recursive_wrapper<expression>
	>
	operand;

	///////////////////////////////////////////////////////////////////////////
	// Operation tokens
	///////////////////////////////////////////////////////////////////////////
	enum op_token
	{
		// cglover-todo: Need to express precedence somehow.
		op_positive,
		op_negative,
		op_not,	

		op_assign,
		op_add,
		op_subtract,
		op_multiply,
		op_divide,

		op_pow,
		op_abs,
	};

	struct unary_op
	{
		op_token  operator_;
		operand	  right;
	};

	struct binary_op
	{
		op_token  operator_;
		operand	  right;
	};

	struct intrinsic_op
	{
		op_token intrinsic;
		std::vector<expression> args;
	};

	///////////////////////////////////////////////////////////////////////////
	// Combination of ops
	///////////////////////////////////////////////////////////////////////////
	struct expression
	{
		operand first;
		std::vector<binary_op> rest;
	};

	// print functions for debugging
	inline std::ostream& operator<<(std::ostream& out, nil)
	{ 
		out << "nil"; return out; 
	}

	inline std::ostream& operator<<(std::ostream& out, identifier const& id)
	{
		out << id; return out;
	}
}}

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::unary_op,
	(evalulater::ast::op_token, operator_)
	(evalulater::ast::operand, right)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::binary_op,
	(evalulater::ast::op_token, operator_)
	(evalulater::ast::operand, right)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::intrinsic_op,
	(evalulater::ast::op_token, intrinsic)
	(std::vector<evalulater::ast::expression>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::expression,
	(evalulater::ast::operand, first)
	(std::vector<evalulater::ast::operand>, rest)
)

#endif // _EVALULATER_AST_HPP_