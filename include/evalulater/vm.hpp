// ****************************************************************************
// evalulater/vm.hpp
//
// Virtual machine for evalulater syntax.  Runs a series of op codes as 
// produced from the compiler.
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
#ifndef _EVALULATER_VM_HPP_
#define _EVALULATER_VM_HPP_
#pragma once

#include "evalulater/config.hpp"
#include "evalulater/byte_code.hpp"

#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/foreach.hpp>
#include <vector>

namespace evalulater { namespace vm
{
	///////////////////////////////////////////////////////////////////////////
	//  The Virtual Machine - Executes the supplied byte code 
	///////////////////////////////////////////////////////////////////////////
    class vmachine
    {
    public:

        vmachine(unsigned stack_size = 4096)
          : stack(stack_size)
          , stack_ptr(stack.begin())
        {}

        float top() const { return stack_ptr[-1]; };
        void execute(std::vector<byte_code> const& code);

    private:

        std::vector<float> stack;
        std::vector<float>::iterator stack_ptr;
    };
}}

#endif //_EVALULATER_VM_HPP_
