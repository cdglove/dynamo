// ****************************************************************************
// dynamo/error_handler.hpp
//
// Parsing error handler for dynamo syntax.
//
// Based on Boost.Spirit samples Copyright (c) 2001-2011 Joel de Guzman
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _DYNAMO_ERRORHANDLER_HPP_
#define _DYNAMO_ERRORHANDLER_HPP_
#pragma once

#include "dynamo/config.hpp"
#include <iostream>
#include <vector>

namespace dynamo
{
	///////////////////////////////////////////////////////////////////////////////
	//  The error handler - pretty prints a helpful error when parsing fails
	///////////////////////////////////////////////////////////////////////////////
	class diagnostic_sink
	{
	public:
		diagnostic_sink(std::ostream& sink)
			: outs(sink)
		{}

		std::ostream& operator()() const 
		{
			return outs;
		}

	protected:
		
		std::ostream& outs;

	private:

		diagnostic_sink operator=(diagnostic_sink);
	};

	template <typename Iterator>
    struct error_handler : diagnostic_sink
    {
        template <typename T0 = void, typename T1 = void, typename T2 = void>
        struct result { typedef void type; };

        error_handler(std::ostream& sink)
          : first(first)
		  , last(last)
		  , diagnostic_sink(sink)
		{}

		void on_parse_begin(Iterator first_, Iterator last_)
		{
			first = first_;
			last = last_;
		}

        template <typename Message, typename What>
        void operator()(
            Message const& message,
            What const& what,
            Iterator err_pos) const
        {
            int line;
            Iterator line_start = get_pos(err_pos, line);
            if (err_pos != last)
            {
                outs << message << what << " line " << line << ':' << std::endl;
                outs << get_line(line_start) << std::endl;
                for (; line_start != err_pos; ++line_start)
                    outs << ' ';
                outs << '^' << std::endl;
            }
            else
            {
                outs << "Unexpected end of file. ";
                outs << message << what << " line " << line << std::endl;
            }
        }

        Iterator get_pos(Iterator err_pos, int& line) const
        {
            line = 1;
            Iterator i = first;
            Iterator line_start = first;
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
            while (i != last && (*i != '\r' && *i != '\n'))
                ++i;
            return std::string(err_pos, i);
        }

        Iterator first;
        Iterator last;
        std::vector<Iterator> iters;
    };
}

#endif //_DYNAMO_ERRORHANDLER_HPP_