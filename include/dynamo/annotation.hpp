// ****************************************************************************
// dynamo/parser/annotation.hpp
//
// Annotation handler for parsing and compiling 
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
#ifndef _DYNAMO_ANNOTATION_HPP_
#define _DYNAMO_ANNOTATION_HPP_
#pragma once

#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/bool.hpp>
#include "dynamo/ast/ast.hpp"

#include <boost/variant/apply_visitor.hpp>
#include <vector>

namespace dynamo
{
    ///////////////////////////////////////////////////////////////////////////////
    //  The annotation handler links the AST to a vector of iterator positions
    //  for the purpose of subsequent semantic error handling when the
    //  program is being compiled.
    ///////////////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    struct annotation
    {
        template <typename T0 = void, typename T1 = void>
        struct result { typedef void type; };

        std::vector<Iterator>& iters;

        annotation(std::vector<Iterator>& iters)
          : iters(iters)
		{}

        struct set_id
        {
            typedef void result_type;

            int id;
            set_id(int id)
				: id(id)
			{}

            void operator()(ast::intrinsic_op& x) const
            {
                x.id = id;
            }

            void operator()(ast::identifier& x) const
            {
                x.id = id;
            }

            template <typename T>
            void operator()(T& x) const
            {
                // no-op
            }
        };

        void operator()(ast::operand& ast, Iterator pos) const
        {
            std::size_t id = iters.size();
            iters.push_back(pos);
            boost::apply_visitor(set_id(id), ast);
        }

		void operator()(ast::intrinsic_op& ast, Iterator pos) const
		{
			std::size_t id = iters.size();
			iters.push_back(pos);
			ast.id = static_cast<int>(id);
		}

        void operator()(ast::assignment& ast, Iterator pos) const
        {
			std::size_t id = iters.size();
            iters.push_back(pos);
            ast.lhs.id = static_cast<int>(id);
        }

        void operator()(ast::identifier& ast, Iterator pos) const
        {
			std::size_t id = iters.size();
            iters.push_back(pos);
            ast.id = static_cast<int>(id);
        }
    };
}

#endif // _DYNAMO_ANNOTATION_HPP_

