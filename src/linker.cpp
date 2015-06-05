// ****************************************************************************
// evalulater/linker.cpp
//
// Implementation of Linker for evalulater syntax.  
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************

#include "evalulater/linker.hpp"
#include "evalulater/vm/byte_code.hpp"
#include <boost/foreach.hpp>
#include <sstream>

namespace evalulater
{
	boost::optional<vm::executable> linker::link(vm::byte_code const& code)
	{
		return vm::executable(code);
	}

	boost::optional<vm::executable> linker::link(vm::byte_code const& code, constant_index const& externs)
	{
		std::vector<float*> variable_table;
		std::vector<float const*> constant_table;
		if(link_constants(code, externs, boost::none, constant_table))
		{
			return vm::executable(code, variable_table, constant_table);
		}

		return boost::none;
	}

	boost::optional<vm::executable> linker::link(vm::byte_code const& code, variable_index& locals)
	{
		std::vector<float*> variable_table;
		std::vector<float const*> constant_table;
		if(link_variables(code, boost::none, locals, variable_table))
		{
			return vm::executable(code, variable_table, constant_table);
		}

		return boost::none;
	}

	boost::optional<vm::executable> linker::link(vm::byte_code const& code, constant_index const& externs, variable_index& locals)
	{
		std::vector<float*> variable_table;
		std::vector<float const*> constant_table;
		
		bool v_ok = link_variables(code, externs, locals, variable_table);
		bool c_ok = link_constants(code, externs, locals, constant_table);
		
		if(v_ok && c_ok)
		{
			return vm::executable(code, variable_table, constant_table);
		}

		return boost::none;
	}

	bool linker::link_constants(vm::byte_code const& code,
							    constant_index const& externs, 
							    boost::optional<variable_index const&> locals,
							    std::vector<float const*>& constant_table)
	{
		vm::data_index const& slot_index = code.get_constants();
		constant_table.resize(slot_index.size());
		BOOST_FOREACH(vm::data_index::value_type const& slot, slot_index)
		{
			// The compiler encodes the reference slot into the instruction stream
			// so we just need to put a pointer into that slot.
			int data_slot = slot.second;
			std::string const& data_name = slot.first;

			constant_index::const_iterator ext = externs.find(data_name);
			
			// Issue linker error, unresolved external (could also use a default
			// here by allocating from the local store).
			if(ext == externs.end())
			{
				// If we have locals see if it's in the local table which it
				// might be if it's not extern.
				if(locals)
				{
					variable_index::const_iterator lcl = locals->find(data_name);
					if(lcl != locals->end())
					{
						constant_table[data_slot] = &lcl->second;
						continue; 
					}
				}

				std::stringstream diagnostic;
				diagnostic << "Undefined external symbol \""
						   << data_name
						   << "\" referenced in "
						   << code.name()
				;

				error_handler(diagnostic.str());
				return false;
			}
			else
			{
				constant_table[data_slot] = ext->second;	
			}
		}

		return true;
	}

	bool linker::link_variables(vm::byte_code const& code,
							    boost::optional<constant_index const&> externs,
							    variable_index& locals, 
							    std::vector<float*>& variable_table)
	{
		vm::data_index const& slot_index = code.get_variables();
		variable_table.resize(slot_index.size());
		BOOST_FOREACH(vm::data_index::value_type const& slot, slot_index)
		{
			// The compiler encodes the reference slot into the instruction stream
			// so we just need to put a pointer into that slot.
			int data_slot = slot.second;
			std::string const& data_name = slot.first;

			// Ensure this local doesnt exist in the extern table
			// Multiply defined symbol if it does.
			if(externs)
			{
				constant_index::const_iterator ext = externs->find(data_name);
				if(ext != externs->end())
				{
					std::stringstream diagnostic;
					diagnostic << "Multiply defined symbol \""
							   << data_name
							   << "\" referenced in "
							   << code.name()
							   << ". Did you mean to create a new variable?"
					;

					error_handler(diagnostic.str());
					return false;
				}
			}

			// We don't need to check if it's there or not
			// just blindly insert which will do nothing if
			// the variable exists.
			variable_index::iterator lcl = locals.insert(std::make_pair(data_name, 0.f)).first;
			variable_table[data_slot] = &lcl->second;	
		}

		return true;
	}
}