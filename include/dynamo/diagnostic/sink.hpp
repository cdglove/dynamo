// ****************************************************************************
// dynamo/diagnostic_sink.hpp
//
// Adapter of an ostream to dynamo internals.
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _DYNAMO_DIAGNOSTICSINK_HPP_
#define _DYNAMO_DIAGNOSTICSINK_HPP_
#pragma once

#include "dynamo/config.hpp"
#include "dynamo/nonassignable.hpp"
#include <iostream>
#include <vector>

namespace dynamo
{
	///////////////////////////////////////////////////////////////////////////////
	//  The diagnostic printer - adapts a stream object to our diagnostic system
	///////////////////////////////////////////////////////////////////////////////
	class diagnostic_sink : nonassignable
	{
	public:
		diagnostic_sink(std::ostream& sink_)
			: outs(sink_)
		{}

		std::ostream& operator()() const 
		{
			return outs;
		}

	protected:

		std::ostream& outs;
	};
}

#endif //_DYNAMO_DIAGNOSTICSINK_HPP_