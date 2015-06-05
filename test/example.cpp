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
#include "evalulater/parser/statement.hpp"
#include "evalulater/compiler.hpp"

#include <iostream>
#include <string>

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

	evalulater::vm::extern_index extern_state;
	float t1 = 5.f;
	float t2 = 11.f;
	extern_state["t1"] = &t1;
	extern_state["t2"] = &t2;

	evalulater::vm::local_index local_state;
	
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
        boost::spirit::ascii::space_type space;
        bool r = phrase_parse(iter, end, +stmt, space, ast);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
			boost::optional<evalulater::vm::byte_code> code = compiler.compile(ast);
			if(code)
			{
				boost::optional<evalulater::vm::executable> exe = 
					evalulater::vm::link(*code, extern_state, local_state)
				;	
				
				if(exe)
				{
					machine.execute(*exe);
					std::cout << "\nResult: " << machine.top() << std::endl;
					std::cout << "-------------------------\n";
				}
				else
				{
					std::cout << "-------------------------\n";
					std::cout << "Linking failed\n";
					std::cout << "-------------------------\n";
				}
			}
			else
			{
				std::cout << "-------------------------\n";
				std::cout << "Compilation failed\n";
				std::cout << "-------------------------\n";
			}
        }
        else
        {
            std::string rest(iter, end);
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}


