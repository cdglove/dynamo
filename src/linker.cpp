// ****************************************************************************
// dynamo/linker.cpp
//
// Implementation of Linker for dynamo syntax.  
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************

#include "dynamo/linker.hpp"
#include "dynamo/vm/byte_code.hpp"
#include "dynamo/error_handler.hpp"
#include <boost/foreach.hpp>
#include <sstream>

namespace dynamo
{
	struct load_float_ptr
	{
		load_float_ptr(float const* f)
			: data_(f)
		{}

		float operator()()
		{
			return *data_;
		}

		float const* data_;
	};

	boost::optional<vm::executable> linker::link(vm::byte_code const& code)
	{
		std::vector<float*> variable_table;
		std::vector<vm::fetch_constant_fun> constant_table;

		// Link variables must come before constants because we might
		// create variables that are later read as constants.
		bool v_ok = link_variables(code, NULL, NULL, variable_table);
		bool c_ok = link_constants(code, NULL, NULL, constant_table);

		if(v_ok && c_ok)
		{
			return vm::executable(code, variable_table, constant_table);
		}

		return boost::none;
	}

	boost::optional<vm::executable> linker::link(vm::byte_code const& code, 
												 constant_index const& constants)
	{
		std::vector<float*> variable_table;
		std::vector<vm::fetch_constant_fun> constant_table;

		// Link variables must come before constants because we might
		// create variables that are later read as constants.
		bool v_ok = link_variables(code, &constants, NULL, variable_table);
		bool c_ok = link_constants(code, &constants, NULL, constant_table);

		if(v_ok && c_ok)
		{
			return vm::executable(code, variable_table, constant_table);
		}

		return boost::none;
	}

	boost::optional<vm::executable> linker::link(vm::byte_code const& code,
												 variable_index& variables)
	{
		std::vector<float*> variable_table;
		std::vector<vm::fetch_constant_fun> constant_table;
		
		// Link variables must come before constants because we might
		// create variables that are later read as constants.
		bool v_ok = link_variables(code, NULL, &variables, variable_table);
		bool c_ok = link_constants(code, NULL, &variables, constant_table);

		if(v_ok && c_ok)
		{
			return vm::executable(code, variable_table, constant_table);
		}

		return boost::none;
	}

	boost::optional<vm::executable> linker::link(vm::byte_code const& code, 
												 constant_index const& constants, 
												 variable_index& variables)
	{
		std::vector<float*> variable_table;
		std::vector<vm::fetch_constant_fun> constant_table;
		
		// Link variables must come before constants because we might
		// create variables that are later read as constants.
		bool v_ok = link_variables(code, &constants, &variables, variable_table);
		bool c_ok = link_constants(code, &constants, &variables, constant_table);
		
		if(v_ok && c_ok)
		{
			return vm::executable(code, variable_table, constant_table);
		}

		return boost::none;
	}

	bool linker::link_constants(
		vm::byte_code const& code,
		constant_index const* constants, 
		variable_index const* variables,
		std::vector<vm::fetch_constant_fun>& constant_table)
	{
		vm::data_index const& slot_index = code.get_constants();
		constant_table.resize(slot_index.size());
		BOOST_FOREACH(vm::data_index::value_type const& slot, slot_index)
		{
			// The compiler encodes the reference slot into the instruction stream
			// so we just need to put a pointer into that slot.
			int data_slot = slot.second;
			std::string const& data_name = slot.first;

			constant_index::const_iterator con;
			
			if(constants)
			{
				con = constants->find(data_name);
			}
			
			// Issue linker error, unresolved external (could also use a default
			// here by allocating from the local store).
			if(!constants || con == constants->end())
			{
				// If we have locals see if it's in the variable table which it
				// might be if it's not a constant.
				if(variables)
				{
					variable_index::const_iterator var = variables->find(data_name);
					if(var != variables->end())
					{
						constant_table[data_slot] = load_float_ptr(&var->second);
						continue; 
					}
				}

				diagnostic() << "Error! Undefined external symbol \""
							 << data_name
							 << "\" referenced in "
							 << code.name()
							 << std::endl
							 ;

				return false;
			}
			else
			{
				constant_table[data_slot] = con->second;	
			}
		}

		return true;
	}

	bool linker::link_variables(
		vm::byte_code const& code,
		constant_index const* constants, 
		variable_index* variables,
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

			// Ensure this local doesnt exist in the constant table
			// Multiply defined symbol if it does.
			if(constants)
			{
				constant_index::const_iterator con = constants->find(data_name);
				if(con != constants->end())
				{
					diagnostic() << "Multiply defined symbol \""
							     << data_name
							     << "\" referenced in "
							     << code.name()
							     << ". Did you mean to create a new variable?"
								 << std::endl
								 ;

					return false;
				}
			}

			if(variables)
			{
				// We don't need to check if it's there or not
				// just blindly insert which will do nothing if
				// the variable exists.
				variable_index::iterator var = variables->insert(std::make_pair(data_name, 0.f)).first;
				variable_table[data_slot] = &var->second;	
			}
			else
			{
				diagnostic() << "Error! Undefined external symbol \""
							 << data_name
							 << "\" referenced in "
							 << code.name()
							 << std::endl
							 ;

				return false;
			}
		}

		return true;
	}
}