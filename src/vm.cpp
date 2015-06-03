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
	state::state(byte_code const& code)
	{}

	state::state(byte_code const& code, std::map<std::string, float*> const& externs_)
	{
		std::map<std::string, int> const& code_externs = code.get_externs();
		externs.resize(code_externs.size());
		BOOST_FOREACH(std::map<std::string, int> ce, code_externs)
		{
			std::map<std::string, float*>::const_iterator i = externs_.find(ce.first);
			if(i == externs_.end())
			{
				// Emit linker error?
				// Allocate one from a small local buffer
			}
			else
			{
				// Link the variable in.
				externs[ce.second] = i->second;
			}
		}
	}
	
	void state::store_extern(int idx, float data)
	{
		*externs[idx] = data;
	}

	float state::load_extern(int idx)
	{
		return *externs[idx];
	}

	void vmachine::execute(byte_code const& code)
	{
		state state;
		execute(code, state);
	}

	void vmachine::execute(byte_code const& code, state& state)
	{
		std::vector<instruction>::const_iterator pc = code.get_code().begin();
		std::vector<instruction>::const_iterator end = code.get_code().end();
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
                *stack_ptr++ = state.load_extern(*pc++);
                break;

            case op_store:
                --stack_ptr;
                state.store_extern(*pc++, stack_ptr[0]);
                break;

			}
		}
	}
}}