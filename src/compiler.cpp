// ****************************************************************************
// dynamo/compiler.cpp
//
// Implementation of Compiler for dynamo syntax.  
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

#include "dynamo/compiler.hpp"
#include "dynamo/ast/ast.hpp"
#include "dynamo/vm/byte_code.hpp"

#include <boost/foreach.hpp>
#include <boost/type_traits/extent.hpp> 
#include <sstream>

namespace dynamo
{
	boost::optional<vm::byte_code> compiler::compile(ast::statement_list const& x)
	{
		vm::byte_code code("anonymous");
		ast_visitor visitor(code, diagnostic, error_handler);
		BOOST_FOREACH(ast::statement const& s, x)
		{
			if(!visitor(s))
				return boost::none;
		}
		return code;
	}

	bool compiler::compile(ast::statement_list const& x, vm::byte_code& out_code)
	{
		ast_visitor visitor(out_code, diagnostic, error_handler);
		BOOST_FOREACH(ast::statement const& s, x)
		{
			if(!visitor(s))
				return false;
		}

		return true;
	}

	bool compiler::ast_visitor::operator()(ast::nil) const
	{ 
		BOOST_ASSERT(0);
		return false;
	}

	bool compiler::ast_visitor::operator()(float f) const
	{
		code.push(vm::op_flt);
		code.push(f);
		return true;
	}

	bool compiler::ast_visitor::operator()(ast::binary_op const& x) const
	{
		if(!boost::apply_visitor(*this, x.right))
			return false;

		return compile_op_token(x.operator_);
	}

	bool compiler::ast_visitor::operator()(ast::unary_op const& x) const
	{
		if(!boost::apply_visitor(*this, x.right))
			return false;

		return compile_op_token(x.operator_);
	}

	static std::size_t const intrinsic_paramcount[] =
	{
	    0, //op_assign,
		2, //op_add,
		2, //op_subtract,
		2, //op_multiply,
		2, //op_divide,
	    0, //op_positive,
	    0, //op_negative,
		1, //op_not,	
		2, //op_pow,
		1, //op_abs,
	};

	BOOST_STATIC_ASSERT(
		sizeof(intrinsic_paramcount) / 
		sizeof(intrinsic_paramcount[0]) == 
		ast::num_op_tokens
	);

	bool compiler::ast_visitor::operator()(ast::intrinsic_op const& x) const
	{
		if(x.args.size() != intrinsic_paramcount[x.intrinsic])
		{
			error_handler(x.id, "Parameter count mismatch.");

			diagnostic() << "Expected "
						 << intrinsic_paramcount[x.intrinsic]
						 << " parameters but was supplied with " 
						 << x.args.size() 
						 << "."
						 << std::endl
						 ;

			return false;
		}

		BOOST_FOREACH(ast::operand const& arg, x.args)
		{
			if(!boost::apply_visitor(*this, arg))
				return false;
		}
		
		return compile_op_token(x.intrinsic);
	}

	bool compiler::ast_visitor::operator()(ast::expression const& x) const
	{
		if(!boost::apply_visitor(*this, x.first))
			return false;

		std::vector<ast::binary_op>::const_iterator begin = x.rest.begin();
		return compile_expression(0, begin, x.rest.end());
	}

	bool compiler::ast_visitor::operator()(ast::assignment const& x) const
	{
		(*this)(x.rhs);
		int const* slot = code.find_variable(x.lhs);
		if(slot == NULL)
		{
			slot = code.add_variable(x.lhs);
		}

		code.push(vm::op_store);
		code.push(*slot);
		return true;
	}

	bool compiler::ast_visitor::operator()(ast::statement const& x) const
	{
		return boost::apply_visitor(*this, x);
	}

	bool compiler::ast_visitor::operator()(ast::identifier const& x) const
	{
		int const* lcl_slot = code.find_variable(x);
		if(lcl_slot)
		{
			code.push(vm::op_load);
			code.push(*lcl_slot);
			return true;
		}

		int const* ext_slot = code.find_constant(x);
		if(ext_slot == NULL)
		{
			ext_slot = code.add_constant(x);
		}

		code.push(vm::op_loadc);
		code.push(*ext_slot);
		return true;
	}

	static int const precedence[] =
	{
		// precedence 1
		1, //op_assign,

		// precedence 2
		2, //op_add,
		2, //op_subtract,

		// precedence 3
		3, //op_multiply,
		3, //op_divide,

		// precedence 4
		4, //op_positive,
		4, //op_negative,
		4, //op_not,	

		// no precedence 
		0, //op_pow,
		0, //op_abs,
	};

	BOOST_STATIC_ASSERT(
		sizeof(precedence) /
		sizeof(precedence[0]) == 
		ast::num_op_tokens
	);

	// The Shunting-yard algorithm -- thanks dykstra
	bool compiler::ast_visitor::compile_expression(
		int min_precedence,
		std::vector<ast::binary_op>::const_iterator& begin,
		std::vector<ast::binary_op>::const_iterator end) const
	{
		while ((begin != end) && (precedence[begin->operator_] >= min_precedence))
		{
			ast::op_token op = begin->operator_;
			if (!boost::apply_visitor(*this, begin->right))
				return false;
			++begin;

			while ((begin != end) && (precedence[begin->operator_] > precedence[op]))
			{
				ast::op_token next_op = begin->operator_;
				compile_expression(precedence[next_op], begin, end);
			}
			compile_op_token(op);
		}
		return true;
	}

	bool compiler::ast_visitor::compile_op_token(ast::op_token token) const
	{
		switch(token)
		{
		case ast::op_add:		code.push(vm::op_add); break; 
		case ast::op_subtract:  code.push(vm::op_sub); break;	
		case ast::op_multiply:  code.push(vm::op_mul); break;
		case ast::op_divide:    code.push(vm::op_div); break;
		case ast::op_pow:		code.push(vm::op_pow); break; 
		case ast::op_abs:		code.push(vm::op_abs); break; 
		case ast::op_not:		code.push(vm::op_not); break;
		case ast::op_negative:	code.push(vm::op_neg); break; 
		case ast::op_positive:				     	   break;	
		default: BOOST_ASSERT(0); return false;
		}

		return true;
	}
}

