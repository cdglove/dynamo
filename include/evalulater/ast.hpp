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
	struct signed_;
	struct expression;

	typedef boost::variant<
		nil
		, float
		, boost::recursive_wrapper<signed_>
		, boost::recursive_wrapper<expression>
	>
	operand;

	struct signed_
	{
		char sign;
		operand operand_;
	};

	struct operation
	{
		char operator_;
		operand operand_;
	};

	struct expression
	{
		operand first;
		std::vector<operation> rest;
	};

	// print function for debugging
	inline std::ostream& operator<<(std::ostream& out, nil) { out << "nil"; return out; }
}}

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::signed_,
	(char, sign)
	(evalulater::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::operation,
	(char, operator_)
	(evalulater::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::expression,
	(evalulater::ast::operand, first)
	(std::vector<evalulater::ast::operation>, rest)
)

#endif // _EVALULATER_AST_HPP_