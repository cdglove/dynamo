// ****************************************************************************
// evalulater/vm.cpp
//
// Implementation of vm for evalulater syntax.  
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

#include "evalulater/vm.hpp"

namespace evalulater { namespace vm
{
	void vmachine::execute(std::vector<byte_code> const& code)
	{
		std::vector<byte_code>::const_iterator pc = code.begin();
		stack_ptr = stack.begin();

		while(pc != code.end())
		{
			op_code op = pc->op;
			++pc;
			switch(op)
			{
			case op_neg:
				stack_ptr[-1] = -stack_ptr[-1];
				break;

			case op_add:
				--stack_ptr;
				stack_ptr[-1] += stack_ptr[0];
				break;

			case op_sub:
				--stack_ptr;
				stack_ptr[-1] -= stack_ptr[0];
				break;

			case op_mul:
				--stack_ptr;
				stack_ptr[-1] *= stack_ptr[0];
				break;

			case op_div:
				--stack_ptr;
				stack_ptr[-1] /= stack_ptr[0];
				break;

			case op_abs:
				stack_ptr[-1] = std::abs(stack_ptr[-1]);
				break;

			case op_not:
				stack_ptr[-1] = !stack_ptr[-1];
				break;
		
			case op_pow:
				--stack_ptr;
				stack_ptr[-1] = std::pow(stack_ptr[-1], stack_ptr[0]);
				break;

			case op_flt:
				*stack_ptr++ = pc->fltd;
				++pc;
				break;
			}
		}
	}
}}