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

#include "evalulater/byte_code.hpp"
#include <boost/assert.hpp>

namespace evalulater { namespace vm
{
	int const* byte_code::add_extern(std::string name)
	{
		BOOST_ASSERT(find_extern(name) == NULL);
		std::size_t n = extern_index_.size();
		return &extern_index_.insert(
			std::make_pair(std::move(name), static_cast<int>(n))
			).first->second;
	}

	int const* byte_code::find_extern(std::string const& name) const
	{
		extern_index::const_iterator i = extern_index_.find(name);
		if (i == extern_index_.end())
			return NULL;
		return &i->second;	
	}
}}