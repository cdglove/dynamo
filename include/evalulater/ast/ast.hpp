// ****************************************************************************
// evalulater/ast/ast.hpp
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
#ifndef _EVALULATER_AST_AST_HPP_
#define _EVALULATER_AST_AST_HPP_
#pragma once

#include "evalulater/config.hpp"

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

	struct expression
	{
		operand first;
		std::vector<binary_op> rest;
	};

	struct assignment
    {
        identifier lhs;
        expression rhs;
    };

	typedef boost::variant<
		  nil
		, assignment
		, expression
	>
	statement;

	typedef std::vector<statement> statement_list;

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

#endif // _EVALULATER_AST_HPP_