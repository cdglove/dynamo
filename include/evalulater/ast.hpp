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
	struct unary;
	struct intrinsic;
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
		, boost::recursive_wrapper<unary>
		, boost::recursive_wrapper<intrinsic>
		, boost::recursive_wrapper<expression>
	>
	operand;

	enum optoken
	{
		// cglover-todo: Need to express precidence somehow.
		op_assign,

		op_abs,
		op_pow,

		op_add,
		op_subtract,

		op_multiply,
		op_divide,

		op_positive,
		op_negative,
	};

	struct unary
	{
		optoken operator_;
		operand operand_;
	};

	struct operation
	{
		optoken operator_;
		operand operand_;
	};

	struct intrinsic
	{
		optoken intr;
		std::vector<expression> args;
	};

	struct expression
	{
		operand first;
		std::vector<operation> rest;
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
	evalulater::ast::unary,
	(evalulater::ast::optoken, operator_)
	(evalulater::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::operation,
	(evalulater::ast::optoken, operator_)
	(evalulater::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::intrinsic,
	(evalulater::ast::optoken, intr)
	(std::vector<evalulater::ast::expression>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::expression,
	(evalulater::ast::operand, first)
	(std::vector<evalulater::ast::operation>, rest)
)

#endif // _EVALULATER_AST_HPP_