// ****************************************************************************
// evalulater/test/linker.cpp
//
// Test linker functionality
// 
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#include "evalulater/vm/machine.hpp"
#include "evalulater/vm/byte_code.hpp"
#include "evalulater/parser/parser.hpp"
#include "evalulater/compiler.hpp"
#include "evalulater/linker.hpp"

#define BOOST_TEST_MODULE Compiler
#include <boost/test/unit_test.hpp>

struct load_float_ptr
{
	load_float_ptr(float const* f)
		: data_(f)
	{}

	float operator()()
	{
		return *data_;
	}

	float const* data_;
};

static void test_expression(
	std::string expression, 
	float expected_result,
	evalulater::constant_index const& external)
{
	evalulater::error_handler<						 
		std::string::const_iterator
	> error_handler(std::cout);	

	evalulater::parse::parser parser(error_handler);
	boost::optional<evalulater::ast::statement_list> ast;
	evalulater::compiler compiler(error_handler);
	boost::optional<evalulater::vm::byte_code> code;
	evalulater::linker linker(error_handler);
	boost::optional<evalulater::vm::executable> exe;
	evalulater::vm::machine vm;

	ast = parser.parse(expression);
	BOOST_CHECK(ast);
	if(ast)
	{
		code = compiler.compile(*ast);
		BOOST_CHECK(code);
		if(code)
		{
			exe = linker.link(*code, external);
			BOOST_CHECK(exe);
			if(exe)
			{
				vm.execute(*exe);
				BOOST_CHECK_EQUAL(vm.top(), expected_result);
			}
		}
	}
}


BOOST_AUTO_TEST_CASE( external_data )
{
	evalulater::constant_index extern_state;
	float t1 = 2.f;
	float t2 = 10.f;
	extern_state["t1"] = load_float_ptr(&t1);
	extern_state["t2"] = load_float_ptr(&t2);

	test_expression("t1 * t2;", 20.f, extern_state);
	test_expression("t3 = t1 * t2;t3;", 20.f, extern_state);
	test_expression("t3 = t1 * t2;t3*t2;", 200.f, extern_state);
}
