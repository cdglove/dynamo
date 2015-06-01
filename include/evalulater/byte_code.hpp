// ****************************************************************************
// evalulater/byte_code.hpp
//
// Defines the byte code produced by the compiler and consumed
// by the vm.
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
#ifndef _EVALULATER_BYTECODE_HPP_
#define _EVALULATER_BYTECODE_HPP_
#pragma once

#include "evalulater/config.hpp"

namespace evalulater { namespace vm
{
	///////////////////////////////////////////////////////////////////////////
	//  Byte code definition
	///////////////////////////////////////////////////////////////////////////
	enum op_code
	{
		op_neg,     //  negate the top stack entry
		op_add,     //  add top two stack entries
		op_sub,     //  subtract top two stack entries
		op_mul,     //  multiply top two stack entries
		op_div,     //  divide top two stack entries
		op_flt,		//  push constant float onto the stack
	};

	union byte_code
	{
		byte_code(op_code v)
			: op(v)
		{}

		byte_code(float v)
			: fltd(v)
		{}

		op_code		 op;
		float		 fltd;
	};
}} // namespace evalulater { namespace vm

#endif // _EVALULATER_BYTECODE_HPP_