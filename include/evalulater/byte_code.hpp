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
#include <vector>
#include <map>

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
		op_abs,		//  calculate abs of the top stack entry
		op_pow,		//  raise stack[-2] to power of the top 
		op_not,		//  logically not the top of the stack 
		op_flt,		//  push constant float onto the stack
		op_load,    //  load a named state variable
        op_store,   //  store a named state variable
	};

	// We can use a union for instructions instead of a variant
	// because the type info is endoded into the op_codes.
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

	class byte_code
	{
	public:

		void push(instruction i) { code.push_back(i); }
		int const* add_extern(std::string name)
		{
			BOOST_ASSERT(find_extern(name) == NULL);
			std::size_t n = variables.size();
			std::map<std::string, int>::iterator v = externs.insert(std::make_pair(name, n));
			return &v->second;
		}
		
		int const* find_extern(std::string const& name) const
		{
			std::map<std::string, int>::const_iterator i = externs.find(name);
			if (i == externs.end())
				return NULL;
			return &i->second;	
		}

		void clear() { code.clear(); externs.clear(); }

		// Some reflection data
		std::vector<instruction> const& get_code() const { return code; }
		std::map<std::string, int> const& get_externs() const { return externs; }

	private:

		std::vector<instruction> code;
		std::map<std::string, int> externs;
	};
}} // namespace evalulater { namespace vm

#endif // _EVALULATER_BYTECODE_HPP_