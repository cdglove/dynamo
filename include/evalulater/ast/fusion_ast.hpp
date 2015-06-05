// ****************************************************************************
// evalulater/ast/fusion_ast.hpp
//
// Fusion adapters of AST to be consumed by parsers.
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
#ifndef _EVALULATER_AST_FUSIONAST_HPP_
#define _EVALULATER_AST_FUSIONAST_HPP_
#pragma once

#include "evalulater/config.hpp"
#include "evalulater/ast/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>

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
	(std::vector<evalulater::ast::binary_op>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
	evalulater::ast::assignment,
	(evalulater::ast::identifier, lhs)
	(evalulater::ast::expression, rhs)
)

#endif // _EVALULATER_AST_FUSIONAST_HPP_