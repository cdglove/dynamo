// ****************************************************************************
// evalulater/compiler.hpp
//
// Compiler for evalulater syntax
// Converts the ast into a series of opcodes to be consumed by the vm.
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
#ifndef _EVALULATER_COMPILER_HPP_
#define _EVALULATER_COMPILER_HPP_
#pragma once

#include "evalulater/config.hpp"
#include "evalulater/ast/ast.hpp"
#include "evalulater/vm/byte_code.hpp"
#include "evalulater/error_handler.hpp"
#include <boost/function.hpp>
#include <boost/optional.hpp>

namespace evalulater
{
	class diagnostic_sink;

	///////////////////////////////////////////////////////////////////////////
	//  The Compiler
	///////////////////////////////////////////////////////////////////////////
	class compiler
	{
	public:

		template <typename ErrorHandler>
        compiler(ErrorHandler& error_handler_)
			: diagnostic(error_handler_)
        {
            error_handler = [&error_handler_](int tag, std::string const& what)
			{
				error_handler_("Error! ", what, error_handler_.iters[tag]);
			};
        }

		boost::optional<vm::byte_code> compile(ast::statement_list const& x);
		bool compile(ast::statement_list const& x, vm::byte_code& out_code);

	private:

		typedef boost::function<
            void(int tag, std::string const& what)
		> error_handler_type;

		struct ast_visitor
		{
			ast_visitor(
				vm::byte_code& code, 
				diagnostic_sink& diagnostic,
				error_handler_type const& error_handler)
				: code(code)
				, diagnostic(diagnostic)
				, error_handler(error_handler)
			{}

			typedef bool result_type;

			bool operator()(ast::nil) const;
			bool operator()(float f) const;
			bool operator()(ast::binary_op const& x) const;
			bool operator()(ast::unary_op const& x) const;
			bool operator()(ast::intrinsic_op const& x) const;
			bool operator()(ast::expression const& x) const;
			bool operator()(ast::assignment const& x) const;
			bool operator()(ast::statement const& x) const;
			bool operator()(ast::identifier const& x) const;

			bool compile_expression(
				int min_precedence,
				std::vector<ast::binary_op>::const_iterator& begin,
				std::vector<ast::binary_op>::const_iterator end) const;

			bool compile_op_token(
				ast::op_token token) const;

			vm::byte_code& code;
			diagnostic_sink& diagnostic;
			error_handler_type const& error_handler;
		};

		diagnostic_sink& diagnostic;
		error_handler_type error_handler;
	};
}

#endif //_EVALULATER_COMPILER_HPP_
