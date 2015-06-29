// ****************************************************************************
// dynamo/compiler.hpp
//
// Compiler for dynamo syntax
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
#ifndef _DYNAMO_COMPILER_HPP_
#define _DYNAMO_COMPILER_HPP_
#pragma once

#include "dynamo/config.hpp"
#include "dynamo/ast/ast.hpp"
#include "dynamo/vm/byte_code.hpp"
#include "dynamo/error_handler.hpp"
#include "dynamo/nonassignable.hpp"
#include <boost/function.hpp>
#include <boost/optional.hpp>

namespace dynamo
{
	class diagnostic_sink;

	///////////////////////////////////////////////////////////////////////////
	//  The Compiler
	///////////////////////////////////////////////////////////////////////////
	class compiler : nonassignable
	{
	public:

        compiler(diagnostic_sink& sink)
			: sink_(sink)
        {
			//error_handler = [&error_handler_](int tag, std::string const& what)
			//{
			//	error_handler_("Error! ", what, error_handler_.iters[tag]);
			//};
			error_handler_ = [](int tag, std::string const& what) {};
        }

		boost::optional<vm::byte_code> compile(ast::statement_list const& x);
		bool compile(ast::statement_list const& x, vm::byte_code& out_code);

	private:

		typedef boost::function<
            void(int tag, std::string const& what)
		> error_handler_type;

		struct ast_visitor : nonassignable
		{
			ast_visitor(
				vm::byte_code& code_, 
				diagnostic_sink& diagnostic_,
				error_handler_type const& error_handler_)
				: code(code_)
				, diagnostic(diagnostic_)
				, error_handler(error_handler_)
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

		diagnostic_sink& sink_;
		error_handler_type error_handler_;
	};
}

#endif //_DYNAMO_COMPILER_HPP_
