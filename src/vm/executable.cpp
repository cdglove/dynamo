// ****************************************************************************
// evalulater/vm.cpp
//
// Implementation of vm for evalulater syntax.  
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

#include "evalulater/vm/machine.hpp"
#include <boost/foreach.hpp>
#include <sstream>

namespace evalulater { namespace vm
{
	executable::executable(byte_code const& code)
		: code(code)
	{}

	executable::executable(byte_code const& code, 
						   std::vector<float*> variable_table_,
						   std::vector<float const*> constant_table_)
		: code(code)
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
		return *constant_table[idx];
	}

	byte_code const& executable::get_code() const
	{
		return code;
	}

	static bool link_constants(byte_code const& code,
							   extern_index const& externs, 
							   std::vector<float const*>& constant_table)
	{
		data_index const& slot_index = code.get_external_refs();
		constant_table.resize(slot_index.size());
		BOOST_FOREACH(data_index::value_type const& slot, slot_index)
		{
			// The compiler encodes the reference slot into the instruction stream
			// so we just need to put a pointer into that slot.
			int data_slot = slot.second;
			std::string const& data_name = slot.first;

			extern_index::const_iterator ext = externs.find(data_name);
			
			// Issue linker error, unresolved external (could also use a default
			// here by allocating from the local store).
			if(ext == externs.end())
			{
				std::stringstream diagnostic;
				diagnostic << "Undefined external symbol \""
							<< data_name
							<< "\" referenced in "
							<< code.name()
				;

				issue_diagnostic(diagnostic.str());
				return false;
			}
			else
			{
				constant_table[data_slot] = ext->second;	
			}
		}

		return true;
	}

	static bool link_variables(byte_code const& code,
							   local_index& locals, 
							   std::vector<float*>& variable_table)
	{
		data_index const& slot_index = code.get_external_refs();
		variable_table.resize(slot_index.size());
		BOOST_FOREACH(data_index::value_type const& slot, slot_index)
		{
			// The compiler encodes the reference slot into the instruction stream
			// so we just need to put a pointer into that slot.
			int data_slot = slot.second;
			std::string const& data_name = slot.first;

			// We don't need to check if it's there or not
			// just blindly insert which will do nothing if
			// the variable exists.
			local_index::iterator ext = locals.insert(std::make_pair(data_name, 0.f)).first;
			variable_table[data_slot] = &ext->second;	
		}

		// Linking locals can never fail
		return true;
	}

	boost::optional<executable> link(byte_code const& code)
	{
		return executable(code);
	}

	boost::optional<executable> link(byte_code const& code, extern_index const& externs)
	{
		std::vector<float*> variable_table;
		std::vector<float const*> constant_table;
		if(link_constants(code, externs, constant_table))
		{
			return executable(code, variable_table, constant_table);
		}

		return boost::optional<executable>();
	}

	boost::optional<executable> link(byte_code const& code, local_index& locals)
	{
		std::vector<float*> variable_table;
		std::vector<float const*> constant_table;
		if(link_variables(code, locals, variable_table))
		{
			return executable(code, variable_table, constant_table);
		}

		return boost::optional<executable>();
	}

	boost::optional<executable> link(byte_code const& code, extern_index const& externs, local_index& locals)
	{
		std::vector<float*> variable_table;
		std::vector<float const*> constant_table;
		
		bool v_ok = link_variables(code, locals, variable_table);
		bool c_ok = link_constants(code, externs, constant_table);
		
		if(v_ok && c_ok)
		{
			return executable(code, variable_table, constant_table);
		}

		return boost::optional<executable>();

	}
}}