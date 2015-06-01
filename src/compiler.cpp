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

	void compiler::operator()(ast::unary_op const& x) const
	{
		boost::apply_visitor(*this, x.operand_);
		switch(x.operator_)
		{
		case ast::uop_negative:	code.push_back(vm::op_neg); break;
		case ast::uop_positive:	break;
		default: BOOST_ASSERT(0); break;
		}
	}

	void compiler::operator()(ast::binary_op const& x) const
	{
		boost::apply_visitor(*this, x.operand_);
		switch (x.operator_)
		{
		case ast::bop_add:		code.push_back(vm::op_add); break; 
		case ast::bop_subtract:	code.push_back(vm::op_sub); break;	
		case ast::bop_multiply:	code.push_back(vm::op_mul); break;
		case ast::bop_divide:	code.push_back(vm::op_div); break;
		default: BOOST_ASSERT(0); break;
		}
	}

	void compiler::operator()(ast::intrinsic_op const& x) const
	{
		BOOST_FOREACH(ast::operand const& arg, x.args)
		{
			boost::apply_visitor(*this, arg);
		}
		
		switch (x.intrinsic)
		{
		case ast::iop_add:		code.push_back(vm::op_add); break; 
		case ast::iop_subtract: code.push_back(vm::op_sub); break;	
		case ast::iop_multiply: code.push_back(vm::op_mul); break;
		case ast::iop_divide:   code.push_back(vm::op_div); break;
		case ast::iop_pow:		code.push_back(vm::op_pow); break; 
		case ast::iop_abs:		code.push_back(vm::op_abs); break; 
		default: BOOST_ASSERT(0); break;
		}
	}

	void compiler::operator()(ast::expression const& x) const
	{
		boost::apply_visitor(*this, x.first);
		BOOST_FOREACH(ast::operand const& oper, x.rest)
		{
			boost::apply_visitor(*this, oper);
		}
	}

	void compiler::operator()(ast::identifier const& x) const
	{}

}