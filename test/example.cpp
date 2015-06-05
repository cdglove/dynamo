// ****************************************************************************
// evalulater/test/example.cpp
//
// Sample program for evalulater expression eveluator.
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
#include "evalulater/parser/statement.hpp"
#include "evalulater/compiler.hpp"
#include "evalulater/linker.hpp"

#include <iostream>
#include <string>

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

// ----------------------------------------------------------------------------
//
int main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "Expression parser...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "Type some expressions...or [q or Q] to quit\n";
	std::cout << "Double line break executes the expressions\n\n";

	typedef std::string::const_iterator iterator_type;

	evalulater::constant_index extern_state;
	float t1 = 5.f;
	float t2 = 11.f;
	extern_state["t1"] = load_float_ptr(&t1);
	extern_state["t2"] = load_float_ptr(&t2);

	evalulater::variable_index local_state;
	
    while(true)
	{
		std::string test_expr;

		// Build up a string with multiple expressions
		std::string line;
		while(std::getline(std::cin, line))
		{
			if (line.empty())
				break;

			if(line[0] == 'q' || line[0] == 'Q')
				return 0;
			
			test_expr += line;
		}

		iterator_type iter = test_expr.begin();
        iterator_type end = test_expr.end();

        evalulater::error_handler<						// Our diagnostic printer
			iterator_type
		> error_handler(iter, end);						

		evalulater::parser::statement<					// Our grammar
			iterator_type
		> stmt(error_handler);

		evalulater::ast::statement_list ast;			// Our program (as AST)
		evalulater::vm::machine machine;				// Our virtual machine
		evalulater::compiler compiler(error_handler);	// Compiles the program
		evalulater::linker linker(error_handler);		// Links the program
        boost::spirit::ascii::space_type space;
        bool r = phrase_parse(iter, end, +stmt, space, ast);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";

			boost::optional<
				evalulater::vm::byte_code
			> code = compiler.compile(ast);

			if(code)
			{
				std::cout << "\nCompiling succeeded\n";
				
				boost::optional<
					evalulater::vm::executable
				> exe = linker.link(*code, extern_state, local_state);	
				
				if(exe)
				{
					std::cout << "\nLinking succeeded\n";

					machine.execute(*exe);

					std::cout << "\nExecution succeeded\n";
					std::cout << "\nResult: " << machine.top() << "\n";
				}
				else
				{
					std::cout << "\nLinking failed\n";
				}
			}
			else
			{
				std::cout << "\nCompilation failed\n";
			}
        }
        else
        {
            std::string rest(iter, end);
            std::cout << "\nParsing failed\n";
        }
		
		std::cout << "-------------------------" << std::endl;
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}


