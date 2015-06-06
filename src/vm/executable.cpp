// ****************************************************************************
// evalulater/vm.cpp
//
// Implementation of vm for evalulater syntax.  
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

#include "evalulater/vm/machine.hpp"
#include <boost/foreach.hpp>

namespace evalulater { namespace vm
{
	executable::executable(byte_code const& code)
		: code(&code)
	{}

	executable::executable(byte_code const& code, 
						   std::vector<float*> variable_table_,
						   std::vector<fetch_constant_fun> constant_table_)
		: code(&code)
	{
		std::swap(variable_table_, variable_table);
		std::swap(constant_table_, constant_table);
	}
	
	void executable::store(int idx, float data)
	{
		*variable_table[idx] = data;
	}

	float executable::load(int idx)
	{
		return *variable_table[idx];
	}

	float executable::loadc(int idx)
	{
		return constant_table[idx]();
	}

	byte_code const& executable::get_code() const
	{
		return *code;
	}
}}