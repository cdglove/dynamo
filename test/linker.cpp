// ****************************************************************************
// dynamo/test/linker.cpp
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
#include "dynamo/vm/machine.hpp"
#include "dynamo/vm/byte_code.hpp"
#include "dynamo/parse/parser.hpp"
#include "dynamo/diagnostic/sink.hpp"
#include "dynamo/compiler.hpp"
#include "dynamo/linker.hpp"

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
	dynamo::constant_index const& external,
	dynamo::variable_index& local)
{
	dynamo::diagnostic_sink diagnostic(std::cout);	

	dynamo::parse::string_parser parser(diagnostic);
	boost::optional<dynamo::ast::statement_list> ast;
	dynamo::compiler compiler(diagnostic);
	boost::optional<dynamo::vm::byte_code> code;
	dynamo::linker linker(diagnostic);
	boost::optional<dynamo::vm::executable> exe;
	dynamo::vm::machine vm;

	ast = parser.parse(expression);
	BOOST_CHECK(ast);
	if(ast)
	{
		code = compiler.compile(*ast);
		BOOST_CHECK(code);
		if(code)
		{
			exe = linker.link(*code, external, local);
			BOOST_CHECK(exe);
			if(exe)
			{
				vm.execute(*exe);
				BOOST_CHECK_EQUAL(vm.top(), expected_result);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE( state_data )
{
	dynamo::constant_index extern_state;
	float t1 = 2.f;
	float t2 = 10.f;
	extern_state["t1"] = load_float_ptr(&t1);
	extern_state["t2"] = load_float_ptr(&t2);

	dynamo::variable_index local_state;

	test_expression("t1 * t2;", 20.f, extern_state, local_state);
	test_expression("t3 = t1 * t2;t3;", 20.f, extern_state, local_state);
	test_expression("t3 = t1 * t2;t3*t2;", 200.f, extern_state, local_state);
	test_expression("t3 = t3 * t2;t3;", 200.f, extern_state, local_state);
}

static void test_fail_linkage(std::string expression)
{
	dynamo::diagnostic_sink diagnostic(std::cout);	

	dynamo::parse::string_parser parser(diagnostic);
	boost::optional<dynamo::ast::statement_list> ast;
	dynamo::compiler compiler(diagnostic);
	boost::optional<dynamo::vm::byte_code> code;
	dynamo::linker linker(diagnostic);
	boost::optional<dynamo::vm::executable> exe;
	dynamo::vm::machine vm;

	ast = parser.parse(expression);
	BOOST_CHECK(ast);
	if(ast)
	{
		code = compiler.compile(*ast);
		BOOST_CHECK(code);
		if(code)
		{
			exe = linker.link(*code);
			BOOST_CHECK(!exe);
		}
	}
}

BOOST_AUTO_TEST_CASE( failed_link )
{
	test_fail_linkage("t1 * t2;");
	test_fail_linkage("t3 = t1 * t2;t3;");
	test_fail_linkage("t3 = t1 * t2;t3*t2;");
	test_fail_linkage("t3 = t3 * t2;t3;");
}