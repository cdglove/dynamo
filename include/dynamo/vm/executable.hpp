// ****************************************************************************
// dynamo/vm/executable.hpp
//
// Executable format for dynamo syntax.  Contains linked
// byte code and variable tables.
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _DYNAMO_VM_EXECUTABLE_HPP_
#define _DYNAMO_VM_EXECUTABLE_HPP_
#pragma once

#include "dynamo/config.hpp"
#include <boost/unordered_map.hpp>
#include <boost/optional.hpp>
#include <boost/function.hpp>
#include <vector>

namespace dynamo
{
	class linker;
}

namespace dynamo { namespace vm
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
		float load(int idx) const;
		float loadc(int idx) const;

		byte_code const& get_code() const;

	private:

		///////////////////////////////////////////////////////////////////////
		// Executables can only be created by the linker.
		///////////////////////////////////////////////////////////////////////
		friend class dynamo::linker;

		executable(
			byte_code const& code, 
			std::vector<float*> variable_table_, 
			std::vector<fetch_constant_fun> constant_table_);

		byte_code const* code;
		std::vector<float*> variable_table;
		std::vector<fetch_constant_fun> constant_table;
	};
}}

#endif //_DYNAMO_VM_EXECUTABLE_HPP_
