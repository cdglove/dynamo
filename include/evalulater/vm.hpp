// ****************************************************************************
// evalulater/vm.hpp
//
// Virtual machine for evalulater syntax.  Based on Boost.Spirit calc6 example
// Runs a series of op codes as produced from the parser.
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _EVALULATER_VM_HPP_
#define _EVALULATER_VM_HPP_
#pragma once

#include "evalulater/config.hpp"
#include "evalulater/ast.hpp"

#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/foreach.hpp>

namespace evalulater
{
    ///////////////////////////////////////////////////////////////////////////
    //  The Virtual Machine - Executes the supplied byte code 
    ///////////////////////////////////////////////////////////////////////////
    enum op_code
    {
        op_neg,     //  negate the top stack entry
        op_add,     //  add top two stack entries
        op_sub,     //  subtract top two stack entries
        op_mul,     //  multiply top two stack entries
        op_div,     //  divide top two stack entries
        op_int,		//  push constant integer onto the stack
		op_flt,		//  push constant float onto
    };

	union byte_code
	{
		byte_code(op_code v)
			: op(v)
		{}

		byte_code(unsigned int v)
			: i(v)
		{}

		op_code		 op;
		unsigned int i;
	};

    class vmachine
    {
    public:

        vmachine(unsigned stack_size = 4096)
          : stack(stack_size)
          , stack_ptr(stack.begin())
        {}

        int top() const { return stack_ptr[-1]; };
        void execute(std::vector<byte_code> const& code);

    private:

        std::vector<int> stack;
        std::vector<int>::iterator stack_ptr;
    };

    void vmachine::execute(std::vector<byte_code> const& code)
    {
        std::vector<byte_code>::const_iterator pc = code.begin();
        stack_ptr = stack.begin();

        while (pc != code.end())
        {
            switch (pc->op)
            {
				++pc;
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

                case op_int:
                    *stack_ptr++ = pc->i;
					++pc;
                    break;
            }
        }
    }
}

#endif //_EVALULATER_VM_HPP_
