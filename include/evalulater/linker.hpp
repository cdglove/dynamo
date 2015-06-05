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
	typedef boost::unordered_map<std::string, float*> extern_index;
	typedef boost::unordered_map<std::string, float>  local_index;

	///////////////////////////////////////////////////////////////////////////
	//  The Linker
	///////////////////////////////////////////////////////////////////////////
	class linker
	{
	public:

		template <typename ErrorHandler>
        linker(ErrorHandler& error_handler_)
        {
            error_handler = [error_handler_](std::string const& what)
			{
				error_handler_("Error! ", what);
			};
        }


		boost::optional<vm::executable> link(vm::byte_code const& code);
		boost::optional<vm::executable> link(vm::byte_code const& code, 
											 extern_index const& externs);
		boost::optional<vm::executable> link(vm::byte_code const& code, 
											 local_index& locals);
		boost::optional<vm::executable> link(vm::byte_code const& code, 
											 extern_index const& externs, 
											 local_index& locals);

	private:

		bool link_constants(vm::byte_code const& code,
							extern_index const& externs, 
							boost::optional<local_index const&>,
							std::vector<float const*>& constant_table);

		bool link_variables(vm::byte_code const& code,
							boost::optional<extern_index const&> externs,
							local_index& locals, 
							std::vector<float*>& variable_table);

		typedef boost::function<
            void(std::string const& what)
		> error_handler_type;

		error_handler_type error_handler;
	};
}

#endif //_EVALULATER_COMPILER_HPP_
