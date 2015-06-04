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

	executable::executable(byte_code const& code, std::vector<float*> variable_table_)
		: code(code)
	{
		std::swap(variable_table_, variable_table);
	}
	
	void executable::store(int idx, float data)
	{
		*variable_table[idx] = data;
	}

	float executable::load(int idx)
	{
		return *variable_table[idx];
	}

	byte_code const& executable::get_code() const
	{
		return code;
	}

	static bool link_variables(
		byte_code const& code,
		extern_index const* externs, 
		local_index* locals,
		std::vector<float*>& variable_table
		)
	{
		typedef boost::unordered_map<std::string, int> code_variables_type;
		code_variables_type const& code_variables = code.get_variables();
		variable_table.resize(code_variables.size());
		BOOST_FOREACH(code_variables_type::value_type const& ce, code_variables)
		{
			// The compiler encodes the reference slot into the instruction stream
			// so we just need to put a pointer into that slot.
			int variable_slot = ce.second;
			std::string const& variable_name = ce.first;

			if(externs)
			{
				extern_index::const_iterator ext = externs->find(variable_name);
				
				// If we don't find a variable with this name, we assume it's a new one
				// and use it instead.
				if(ext != externs->end())
				{
					// Check that the variable does not exist also in locals.
					local_index::iterator lcl = locals->find(variable_name);
					if(lcl != locals->end())
					{
						std::stringstream diagnostic;
						diagnostic << "Multiple defined symbol \""
								   << variable_name
								   << "\" found in both locals and externs."
						;

						issue_diagnostic(diagnostic.str());
						return false;
					}
					// Link the variable in.
					variable_table[variable_slot] = ext->second;
					continue;
				}
			}

			if(locals)
			{
				local_index::iterator lcl = locals->find(variable_name);
				if(lcl == locals->end())
				{
					lcl = locals->insert(std::make_pair(variable_name, 0.f)).first;
				}
				variable_table[variable_slot] = &lcl->second;	
			}
		}

		return true;
	}

	boost::optional<executable> link(byte_code const& code)
	{
		return executable(code);
	}

	boost::optional<executable> link(byte_code const& code, extern_index const& externs)
	{
		std::vector<float*> variable_table;
		if(link_variables(code, &externs, NULL, variable_table))
		{
			return executable(code, variable_table);
		}

		return boost::optional<executable>();
	}

	boost::optional<executable> link(byte_code const& code, local_index& locals)
	{
		std::vector<float*> variable_table;
		if(link_variables(code, NULL, &locals, variable_table))
		{
			return executable(code, variable_table);
		}

		return boost::optional<executable>();
	}

	boost::optional<executable> link(byte_code const& code, extern_index const& externs, local_index& locals)
	{
		std::vector<float*> variable_table;
		if(link_variables(code, &externs, &locals, variable_table))
		{
			return executable(code, variable_table);
		}

		return boost::optional<executable>();
	}
}}