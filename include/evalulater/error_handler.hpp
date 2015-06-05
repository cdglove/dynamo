// ****************************************************************************
// evalulater/error_handler.hpp
//
// Parsing error handler for evalulater syntax.
// Based on Boost.Spirit calc6 example
//
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _EVALULATER_ERRORHANDLER_HPP_
#define _EVALULATER_ERRORHANDLER_HPP_
#pragma once

#include "evalulater/config.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace evalulater
{
	///////////////////////////////////////////////////////////////////////////////
	//  The error handler - pretty prints a helpful error when parsing fails
	///////////////////////////////////////////////////////////////////////////////
	template <typename Iterator>
    struct error_handler
    {
        template <typename T0 = void, typename T1 = void, typename T2 = void>
        struct result { typedef void type; };

        error_handler(Iterator first, Iterator last)
          : first(first), last(last) {}

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
                std::cout << message << what << " line " << line << ':' << std::endl;
                std::cout << get_line(line_start) << std::endl;
                for (; line_start != err_pos; ++line_start)
                    std::cout << ' ';
                std::cout << '^' << std::endl;
            }
            else
            {
                std::cout << "Unexpected end of file. ";
                std::cout << message << what << " line " << line << std::endl;
            }
        }

		template <typename Message, typename What>
        void operator()(
            Message const& message,
            What const& what) const
        {
            std::cout << message << what << std::endl;
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

#endif //_EVALULATER_ERRORHANDLER_HPP_