// ****************************************************************************
// evalulater/vm/byte_code.hpp
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
#ifndef _EVALULATER_VM_BYTECODE_HPP_
#define _EVALULATER_VM_BYTECODE_HPP_
#pragma once

#include "evalulater/config.hpp"
#include <boost/unordered_map.hpp>
#include <boost/utility/string_ref.hpp>
#include <vector>

namespace evalulater { namespace vm
{
	///////////////////////////////////////////////////////////////////////////
	//  Byte code operations
	///////////////////////////////////////////////////////////////////////////
	enum op_code
	{
		op_neg,     //  negate the top stack entry
		op_add,     //  add top two stack entries
		op_sub,     //  subtract top two stack entries
		op_mul,     //  multiply top two stack entries
		op_div,     //  divide top two stack entries
		op_abs,		//  calculate abs of the top stack entry
		op_pow,		//  raise stack[-2] to power of the top 
		op_not,		//  logically not the top of the stack 
		op_flt,		//  push constant float onto the stack
		op_load,    //  load a named local variable
        op_store,   //  store a named local variable
		op_loadc,   //  load a named extern constant
	};

	///////////////////////////////////////////////////////////////////////////
	// Encoded instructions
	// We can use a union for instructions instead of a variant
	// because the type info is encoded into the op_codes.
	///////////////////////////////////////////////////////////////////////////
	union instruction
	{
		instruction(op_code v)
			: op(v)
		{}

		instruction(float v)
			: fltd(v)
		{}

		instruction(int v)
			: intd(v)
		{}

		op_code		 op;
		float		 fltd;
		int			 intd;
	};

	///////////////////////////////////////////////////////////////////////////
	typedef boost::unordered_map<std::string, int> data_index;

	///////////////////////////////////////////////////////////////////////////
	// Byte code object
	// Contains the compiled code plus reflected data and anything else we 
	// need to know post compilation
	///////////////////////////////////////////////////////////////////////////
	class byte_code
	{
	public:

		byte_code(std::string name)
			: name_(name)
		{}
		
		boost::string_ref name() const;
		
		void push(instruction i);

		int const* add_constant(std::string name);
		int const* find_constant(std::string const& name) const;
		int const* add_variable(std::string name);
		int const* find_variable(std::string const& name) const;

		std::vector<instruction> const& get_instructions() const;
		data_index const& get_constants() const;
		data_index const& get_variables() const;

	private:

		std::vector<instruction> code;
		
		std::string name_;
		data_index constant_index_;
		data_index variable_index_;
	};

	inline void byte_code::push(instruction i)
	{ 
		code.push_back(i); 
	}

	///////////////////////////////////////////////////////////////////////////
	// Some Reflected information
	inline boost::string_ref byte_code::name() const
	{
		return name_;
	}

	inline std::vector<instruction> const& byte_code::get_instructions() const
	{
		return code;
	}

	inline boost::unordered_map<std::string, int> const& byte_code::get_constants() const
	{
		return constant_index_;
	}

	inline boost::unordered_map<std::string, int> const& byte_code::get_variables() const
	{
		return variable_index_;
	}
}} // namespace evalulater { namespace vm

#endif // _EVALULATER_VM_BYTECODE_HPP_