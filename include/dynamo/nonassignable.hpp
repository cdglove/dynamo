// ****************************************************************************
// dynamo/nonassignable.hpp
//
// Utility to prevent warning on some compilers that an assignment operator
// could not be automatically generated.
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _DYNAMO_NONASSIGNABLE_HPP_
#define _DYNAMO_NONASSIGNABLE_HPP_
#pragma once

namespace dynamo
{
	class nonassignable
	{
	protected:
		nonassignable()
		{}

		~nonassignable()
		{}

	private:  // emphasize the following members are private
		nonassignable operator=(nonassignable);
	};
}

#endif // _DYNAMO_NONASSIGNABLE_HPP_