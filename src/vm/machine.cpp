// ****************************************************************************
// dynamo/vm.cpp
//
// Implementation of vm for dynamo syntax.  
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

#include "dynamo/vm/machine.hpp"
#include "dynamo/vm/byte_code.hpp"

namespace dynamo { namespace vm
{
	void machine::execute(executable& ex)
	{
		std::vector<instruction> const& instructions = ex.get_code().get_instructions();
		std::vector<instruction>::const_iterator pc = instructions.begin();
		std::vector<instruction>::const_iterator end = instructions.end();
		stack_ptr = stack.begin();

		while(pc != end)
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

            case op_load:
                *stack_ptr++ = ex.load(pc->intd);
				++pc;
                break;

            case op_store:
                --stack_ptr;
                ex.store(pc->intd, stack_ptr[0]);
				++pc;
                break;

			case op_loadc:
				*stack_ptr++ = ex.loadc(pc->intd);
				++pc;
				break;
			}
		}
	}
}}