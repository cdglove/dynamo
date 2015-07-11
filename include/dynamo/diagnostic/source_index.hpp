// ****************************************************************************
// dynamo/source.hpp
//
// Source code package for parsing and compiling.
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _DYNAMO_SOURCE_HPP_
#define _DYNAMO_SOURCE_HPP_
#pragma once

#include "dynamo/config.hpp"
#include "dynamo/diagnostic/sink.hpp"
#include <vector>

namespace dynamo
{
	class source_index_base
	{
	public:

		virtual void operator()(
			diagnostic_sink& sink,
			char const* message,
			char const* what,
			int iterator_idx) const 
		{}
	};

	///////////////////////////////////////////////////////////////////////////////
	// Source package.  Defines the being and end iterators end maintans a maps
	// if id's to iterators.
	///////////////////////////////////////////////////////////////////////////////
	template<typename Iterator>
	class source_index : public source_index_base
	{
	public:

		void begin_parse(Iterator first, Iterator last)
		{
			first_ = first;
			last_ = last;
			identifier_iterators_.clear();
		}

		virtual void operator()(
			diagnostic_sink& sink,
			char const* message,
			char const* what,
			int iterator_idx) const
		{
			Iterator err_pos = identifier_iterators_[iterator_idx];

			int line = 0;
			Iterator line_start = get_pos(err_pos, line);
			if (err_pos != last_)
			{
				sink() << message << ' ' << what << " line " << line << ':' << std::endl;
				sink() << get_line(line_start) << std::endl;
				for (; line_start != err_pos; ++line_start)
					sink() << ' ';
				sink() << '^' << std::endl;
			}
			else
			{
				sink() << "Unexpected end of file. ";
				sink() << message << what << " line " << line << std::endl;
			}
		}


		typedef std::vector<Iterator> index_type;
		index_type identifier_iterators_;
	
		Iterator get_pos(Iterator err_pos, int& line) const
		{
			line = 1;
			Iterator i = first_;
			Iterator line_start = first_;
			while (i != err_pos)
			{
				bool eol = false;
				if (i != err_pos && *i == '\r') // CR
				{
					eol = true;
					line_start = ++i;
				}
				if (i != err_pos && *i == '\n') // LF
				{
					eol = true;
					line_start = ++i;
				}
				if (eol)
					++line;
				else
					++i;
			}
			return line_start;
		}

		std::string get_line(Iterator err_pos) const
		{
			Iterator i = err_pos;
			// position i to the next EOL
			while (i != last_ && (*i != '\r' && *i != '\n'))
				++i;
			return std::string(err_pos, i);
		}
	
		Iterator first_;
		Iterator last_;
	};
}

#endif // _DYNAMO_SOURCE_HPP_