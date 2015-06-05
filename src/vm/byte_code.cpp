// ****************************************************************************
// evalulater/byte_code.cpp
//
// Byte code result at the end of compilation.
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************

#include "evalulater/vm/byte_code.hpp"
#include <boost/assert.hpp>

namespace evalulater { namespace vm
{
	static int const* find_named_ref(data_index const& idx, std::string const& name)
	{
		data_index::const_iterator i = idx.find(name);
		if (i == idx.end())
			return NULL;
		return &i->second;	
	}

	static int const* add_named_ref(data_index& idx, std::string const& name)
	{
		BOOST_ASSERT(find_named_ref(idx, name) == NULL);
		std::size_t n = idx.size();
		return &idx.insert(
			std::make_pair(name, static_cast<int>(n))
			).first->second;
	}

	int const* byte_code::add_external_ref(std::string name)
	{
		return add_named_ref(extern_index_, name);
	}	
	
	int const* byte_code::find_external_ref(std::string const& name) const
	{
		return find_named_ref(extern_index_, name);
	}

	int const* byte_code::add_local_variable(std::string name)
	{
		return add_named_ref(local_index_, name);
	}

	int const* byte_code::find_local_variable(std::string const& name) const
	{
		return find_named_ref(local_index_, name);
	}
}}