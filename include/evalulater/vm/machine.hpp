// ****************************************************************************
// evalulater/vm/machine.hpp
//
// Virtual machine for evalulater syntax.  Runs a series of op codes as 
// produced from the compiler.
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
#ifndef _EVALULATER_VM_MACHINE_HPP_
#define _EVALULATER_VM_MACHINE_HPP_
#pragma once

#include "evalulater/config.hpp"
#include "evalulater/vm/executable.hpp"

#include <vector>

namespace evalulater { namespace vm
{
	///////////////////////////////////////////////////////////////////////////
	//  The Virtual Machine - Executes the supplied byte code 
	///////////////////////////////////////////////////////////////////////////
    class machine
    {
    public:

        machine(unsigned stack_size = 4096)
          : stack(stack_size)
          , stack_ptr(stack.begin())
        {}

        float top() const { return stack_ptr[-1]; };
        void execute(executable& ex);

    private:

        std::vector<float> stack;
        std::vector<float>::iterator stack_ptr;
    };
}}

#endif //_EVALULATER_VM_HPP_
