// ****************************************************************************
// evalulater/compiler.cpp
//
// Implementation of Compiler for evalulater syntax.  
//
// Originally based on Boost.Spirit calc6 example
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************

#include "evalulater/compiler.hpp"
#include "evalulater/ast.hpp"
#include "evalulater/vm.hpp"

#include <boost/foreach.hpp>

namespace evalulater
{
	void compiler::operator()(ast::nil) const
	{ 
		BOOST_ASSERT(0);
	}

	void compiler::operator()(float f) const
	{
		code.push_back(vm::op_flt);
		code.push_back(f);
	}

	void compiler::operator()(ast::operation const& x) const
	{
		boost::apply_visitor(*this, x.operand_);
		switch (x.operator_)
		{
		case ast::op_add:	   code.push_back(vm::op_add); break;
		case ast::op_subtract: code.push_back(vm::op_sub); break;
		case ast::op_multiply: code.push_back(vm::op_mul); break;
		case ast::op_divide:   code.push_back(vm::op_div); break;
		default: BOOST_ASSERT(0); break;
		}
	}

	void compiler::operator()(ast::unary const& x) const
	{
		boost::apply_visitor(*this, x.operand_);
		switch (x.operator_)
		{
		case ast::op_negative: code.push_back(vm::op_neg); break;
		case ast::op_positive: break;
		default: BOOST_ASSERT(0); break;
		}
	}

	void compiler::operator()(ast::expression const& x) const
	{
		boost::apply_visitor(*this, x.first);
		BOOST_FOREACH(ast::operation const& oper, x.rest)
		{
			(*this)(oper);
		}
	}

	void compiler::operator()(ast::identifier const& x) const
	{}

	void compiler::operator()(ast::intrinsic const& x) const
	{}
}