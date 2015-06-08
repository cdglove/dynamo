// ****************************************************************************
// dynamo/ast/ast.hpp
//
// Abstract syntax tree for dynamo syntax.  
// Defines expressions and the data asociated with them.
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
#ifndef _DYNAMO_AST_AST_HPP_
#define _DYNAMO_AST_AST_HPP_
#pragma once

#include "dynamo/config.hpp"

#include <boost/variant/recursive_variant.hpp>
#include <iostream>
#include <vector>

namespace dynamo { namespace ast
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

		int id;
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
		// precedence 1
		op_assign,
		
		// precedence 2
		op_add,
		op_subtract,
		
		// precedence 3
		op_multiply,
		op_divide,
		
		// precedence 4
		op_positive,
		op_negative,
		op_not,	

		// no precedence 
		op_pow,
		op_abs,
		
		num_op_tokens
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
		int id;
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

#endif // _DYNAMO_AST_HPP_