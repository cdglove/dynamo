// ****************************************************************************
// evalulater/linker.hpp
//
// Linker for evalulater syntax
// links byte_code and data into an executable.
// 
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _EVALULATER_LINKER_HPP_
#define _EVALULATER_LINKER_HPP_
#pragma once

#include "evalulater/config.hpp"
#include "evalulater/vm/executable.hpp"
#include <boost/function.hpp>
#include <boost/optional.hpp>

namespace evalulater
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
	class linker
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
			constant_index const& constants, 
			boost::optional<variable_index const&>,
			std::vector<vm::fetch_constant_fun>& constant_table);

		bool link_variables(
			vm::byte_code const& code,
			boost::optional<constant_index const&> externs,
			variable_index& variables, 
			std::vector<float*>& variable_table);

		typedef boost::function<
            void(std::string const& what)
		> error_handler_type;

		diagnostic_sink& diagnostic;
	};
}

#endif //_EVALULATER_COMPILER_HPP_
