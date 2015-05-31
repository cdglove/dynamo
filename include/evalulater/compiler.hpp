// ****************************************************************************
// evalulater/compiler.hpp
//
// Compiler for evalulater syntax.  Based on Boost.Spirit calc6 example
// Generated a series of op codes to be consumed by the vm.
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _EVALULATER_COMPILER_HPP_
#define _EVALULATER_COMPILER_HPP_
#pragma once

#include "evalulater/config.hpp"
#include "evalulater/ast.hpp"
#include "evalulater/vm.hpp"

#include <boost/foreach.hpp>
#include <vector>

namespace evalulater
{
	///////////////////////////////////////////////////////////////////////////
	//  The Compiler
	///////////////////////////////////////////////////////////////////////////
	struct compiler
	{
		typedef void result_type;

		std::vector<byte_code>& code;
		compiler(std::vector<byte_code>& code)
			: code(code) 
		{}

		void operator()(ast::nil) const
		{ 
			BOOST_ASSERT(0);
		}
		
		void operator()(float f) const
		{
			code.push_back(op_flt);
			code.push_back(f);
		}

		void operator()(ast::operation const& x) const
		{
			boost::apply_visitor(*this, x.operand_);
			switch (x.operator_)
			{
			case '+': code.push_back(op_add); break;
			case '-': code.push_back(op_sub); break;
			case '*': code.push_back(op_mul); break;
			case '/': code.push_back(op_div); break;
			default: BOOST_ASSERT(0); break;
			}
		}

		void operator()(ast::signed_ const& x) const
		{
			boost::apply_visitor(*this, x.operand_);
			switch (x.sign)
			{
			case '-': code.push_back(op_neg); break;
			case '+': break;
			default: BOOST_ASSERT(0); break;
			}
		}

		void operator()(ast::expression const& x) const
		{
			boost::apply_visitor(*this, x.first);
			BOOST_FOREACH(ast::operation const& oper, x.rest)
			{
				(*this)(oper);
			}
		}
	};
}

#endif //_EVALULATER_COMPILER_HPP_
