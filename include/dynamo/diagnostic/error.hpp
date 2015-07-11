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
#include "dynamo/nonassignable.hpp"
#include "dynamo/diagnostic/source_index.hpp"
#include "dynamo/diagnostic/sink.hpp"
#include <iostream>
#include <vector>

namespace dynamo
{
	///////////////////////////////////////////////////////////////////////////////
	//  The diagnostic handler - pretty prints a helpful error when parsing fails
	///////////////////////////////////////////////////////////////////////////////
	template <typename Iterator>
    struct error
    {
        template <typename T0 = void, typename T1 = void, typename T2 = void>
        struct result { typedef void type; };

		error(source_index<Iterator>& src, diagnostic_sink& sink)
		  : src_(src)
		  , sink_(sink)
		{}

		template<typename Message, typename What>
        void operator()(
            Message const& message,
            What const& what,
            Iterator err_pos) const
        {
            int line = 0;
            Iterator line_start = get_pos(err_pos, line);
            if (err_pos != src_.last_)
            {
                sink_() << message << ' ' << what << " line " << line << ':' << std::endl;
				sink_() << get_line(line_start) << std::endl;
                for (; line_start != err_pos; ++line_start)
					sink_() << ' ';
				sink_() << '^' << std::endl;
            }
            else
            {
                sink_() << "Unexpected end of file. ";
                sink_() << message << what << " line " << line << std::endl;
            }
        }

        Iterator get_pos(Iterator err_pos, int& line) const
        {
            line = 1;
            Iterator i = src_.first_;
            Iterator line_start = src_.first_;
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
            while (i != src_.last_ && (*i != '\r' && *i != '\n'))
                ++i;
            return std::string(err_pos, i);
        }

		source_index<Iterator>& src_;
		diagnostic_sink& sink_;
    };
}

#endif //_DYNAMO_ERRORHANDLER_HPP_