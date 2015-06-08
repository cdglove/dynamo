// ****************************************************************************
// dynamo/ast/fusion_ast.hpp
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
#ifndef _DYNAMO_AST_FUSIONAST_HPP_
#define _DYNAMO_AST_FUSIONAST_HPP_
#pragma once

#include "dynamo/config.hpp"
#include "dynamo/ast/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	dynamo::ast::unary_op,
	(dynamo::ast::op_token, operator_)
	(dynamo::ast::operand, right)
)

BOOST_FUSION_ADAPT_STRUCT(
	dynamo::ast::binary_op,
	(dynamo::ast::op_token, operator_)
	(dynamo::ast::operand, right)
)

BOOST_FUSION_ADAPT_STRUCT(
	dynamo::ast::intrinsic_op,
	(dynamo::ast::op_token, intrinsic)
	(std::vector<dynamo::ast::expression>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	dynamo::ast::expression,
	(dynamo::ast::operand, first)
	(std::vector<dynamo::ast::binary_op>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
	dynamo::ast::assignment,
	(dynamo::ast::identifier, lhs)
	(dynamo::ast::expression, rhs)
)

#endif // _DYNAMO_AST_FUSIONAST_HPP_