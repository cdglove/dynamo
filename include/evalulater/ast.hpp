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

	struct identifier
	{
		identifier(std::string const& name = "") 
			: name(name) 
		{}
		
		std::string name;
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
	// Binary ops
	///////////////////////////////////////////////////////////////////////////
	enum bop_token
	{
		// cglover-todo: Need to express precedence somehow.
		bop_assign,
		bop_add,
		bop_subtract,
		bop_multiply,
		bop_divide,
	};

	struct binary_op
	{
		bop_token operator_;
		operand	  operand_;
	};

	///////////////////////////////////////////////////////////////////////////
	// Unary ops
	///////////////////////////////////////////////////////////////////////////
	enum uop_token
	{
		uop_positive,
		uop_negative,
	};

	struct unary_op
	{
		uop_token operator_;
		operand	  operand_;
	};

	///////////////////////////////////////////////////////////////////////////
	// Intrinsic ops
	///////////////////////////////////////////////////////////////////////////
	enum iop_token
	{
		iop_add,
		iop_subtract,
		iop_multiply,
		iop_divide,
		iop_pow,
		iop_abs,
	};

	struct intrinsic_op
	{
		iop_token intrinsic;
		std::vector<operand> args;
	};

	///////////////////////////////////////////////////////////////////////////
	// Combination of ops
	///////////////////////////////////////////////////////////////////////////
	struct expression
	{
		operand first;
		std::vector<operand> rest;
	};

	// print functions for debugging
	inline std::ostream& operator<<(std::ostream& out, nil)
	{ 
		out << "nil"; return out; 
	}

	inline std::ostream& operator<<(std::ostream& out, identifier const& id)
	{
		out << id.name; return out;
	}
}}

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::unary_op,
	(evalulater::ast::uop_token, operator_)
	(evalulater::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::binary_op,
	(evalulater::ast::bop_token, operator_)
	(evalulater::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::intrinsic_op,
	(evalulater::ast::iop_token, intrinsic)
	(std::vector<evalulater::ast::operand>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::expression,
	(evalulater::ast::operand, first)
	(std::vector<evalulater::ast::operand>, rest)
)

#endif // _EVALULATER_AST_HPP_