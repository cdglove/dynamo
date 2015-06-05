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
#include "evalulater/vm/byte_code.hpp"
#include <boost/unordered_map.hpp>
#include <boost/optional.hpp>
#include <vector>

namespace evalulater { namespace vm
{
	typedef boost::unordered_map<std::string, float*> extern_index;
	typedef boost::unordered_map<std::string, float>  local_index;

	///////////////////////////////////////////////////////////////////////////
	//  Executable - Contains byte_code and variable lookup information
	///////////////////////////////////////////////////////////////////////////
	class executable
	{
	public:

		void  store(int idx, float data);
		float load(int idx);

		byte_code const& get_code() const;

	private:

		///////////////////////////////////////////////////////////////////////
		// Factory functions to create an executable.  Returns an empty
		// optional if linking fails.
		///////////////////////////////////////////////////////////////////////
		friend boost::optional<executable> link(byte_code const& code);
		friend boost::optional<executable> link(byte_code const& code, 
												extern_index const& externs);
		friend boost::optional<executable> link(byte_code const& code, 
												local_index& locals);
		friend boost::optional<executable> link(byte_code const& code, 
												extern_index const& externs, 
												local_index& locals);

		executable(byte_code const& code);
		executable(byte_code const& code, 
				   std::vector<float*> variable_table_, 
				   std::vector<float*> constant_table_);

		byte_code const& code;
		std::vector<float*> variable_table;
		std::vector<float*> constant_table;
	};
}}

#endif //_EVALULATER_VM_EXECUTABLE_HPP_
