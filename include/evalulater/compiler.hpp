// ****************************************************************************
// evalulater/compiler.hpp
//
// Compiler for evalulater syntax
// Converts the ast into a series of opcodes to be consumed by the vm.
// 
// Originally based on Boost.Spirit calc6 example
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
#include "evalulater/ast.hpp"
#include "evalulater/byte_code.hpp"
#include <vector>

namespace evalulater
{
	///////////////////////////////////////////////////////////////////////////
	//  The Compiler
	///////////////////////////////////////////////////////////////////////////
	struct compiler
	{
		typedef void result_type;

		std::vector<vm::byte_code>& code;
		compiler(std::vector<vm::byte_code>& code)
			: code(code) 
		{}

		void operator()(ast::nil) const;
		void operator()(float f) const;
		void operator()(ast::operation const& x) const;
		void operator()(ast::unary const& x) const;
		void operator()(ast::expression const& x) const;
		void operator()(ast::identifier const& x) const;
		void operator()(ast::intrinsic const& x) const;
	};
}

#endif //_EVALULATER_COMPILER_HPP_
