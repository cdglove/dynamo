// ****************************************************************************
// evalulater/vm/executable.hpp
//
// Executable format for evalulater syntax.  Contains linked
// byte code and variable tables.
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
#ifndef _EVALULATER_VM_EXECUTABLE_HPP_
#define _EVALULATER_VM_EXECUTABLE_HPP_
#pragma once

#include "evalulater/config.hpp"
#include <boost/unordered_map.hpp>
#include <boost/optional.hpp>
#include <boost/function.hpp>
#include <vector>

namespace evalulater
{
	class linker;
}

namespace evalulater { namespace vm
{
	class byte_code;
	typedef boost::function<float()> fetch_constant_fun;

	///////////////////////////////////////////////////////////////////////////
	//  Executable - Contains byte_code and variable lookup information
	///////////////////////////////////////////////////////////////////////////
	class executable
	{
	public:

		void  store(int idx, float data);
		float load(int idx);
		float loadc(int idx);

		byte_code const& get_code() const;

	private:

		///////////////////////////////////////////////////////////////////////
		// Executables can only be created by the linker.
		///////////////////////////////////////////////////////////////////////
		friend class evalulater::linker;

		executable(byte_code const& code);
		executable(byte_code const& code, 
				   std::vector<float*>		       variable_table_, 
				   std::vector<fetch_constant_fun> constant_table_);

		byte_code const* code;
		std::vector<float*> variable_table;
		std::vector<fetch_constant_fun> constant_table;
	};
}}

#endif //_EVALULATER_VM_EXECUTABLE_HPP_
