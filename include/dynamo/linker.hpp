// ****************************************************************************
// dynamo/linker.hpp
//
// Linker for dynamo syntax
// links byte_code and data into an executable.
// 
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _DYNAMO_LINKER_HPP_
#define _DYNAMO_LINKER_HPP_
#pragma once

#include "dynamo/config.hpp"
#include "dynamo/vm/executable.hpp"
#include "dynamo/nonassignable.hpp"
#include <boost/function.hpp>
#include <boost/optional.hpp>

namespace dynamo
{
	class diagnostic_sink;

	typedef boost::unordered_map<
		std::string, vm::fetch_constant_fun
	> constant_index;
	
	typedef boost::unordered_map<
		std::string, float
	> variable_index;

	///////////////////////////////////////////////////////////////////////////
	//  The Linker
	///////////////////////////////////////////////////////////////////////////
	class linker : nonassignable
	{
	public:

	    linker(diagnostic_sink& sink)
			: diagnostic(sink)
		{}

		boost::optional<vm::executable> link(
			vm::byte_code const& code);
		boost::optional<vm::executable> link(
			vm::byte_code const& code, 			
			constant_index const& externs);
		boost::optional<vm::executable> link(
			vm::byte_code const& code, 
			variable_index& locals);
		boost::optional<vm::executable> link(
			vm::byte_code const& code, 
			constant_index const& externs, 
			variable_index& locals);

	private:

		bool link_constants(
			vm::byte_code const& code,
			constant_index const* constants, 
			variable_index const* variables,
			std::vector<vm::fetch_constant_fun>& constant_table);

		bool link_variables(
			vm::byte_code const& code,
			constant_index const* constants, 
			variable_index* variables,
			std::vector<float*>& variable_table);

		typedef boost::function<
            void(std::string const& what)
		> error_handler_type;

		diagnostic_sink& diagnostic;
	};
}

#endif //_DYNAMO_COMPILER_HPP_
