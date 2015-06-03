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

#include "evalulater/vm.hpp"
#include "evalulater/parser.hpp"
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

		evalulater::parser<								// Our grammar
			iterator_type
		> exp(error_handler);

        std::vector<evalulater::ast::expression> ast;	// Our program (as AST)
		evalulater::vm::vmachine machine;				// Our virtual machine
		evalulater::compiler compiler(error_handler);	// Compiles the program
        boost::spirit::ascii::space_type space;
        bool r = phrase_parse(iter, end, +exp, space, ast);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
			BOOST_FOREACH(evalulater::ast::expression& e, ast)
			{
				evalulater::vm::byte_code code = compiler.compile(e);
				machine.execute(code, state);
			}
            std::cout << "\nResult: " << machine.top() << std::endl;
            std::cout << "-------------------------\n";
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


