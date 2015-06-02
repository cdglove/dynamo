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
    std::cout << "Type an expression...or [q or Q] to quit\n\n";

	typedef std::string::const_iterator iterator_type;
	
    std::string test_expr;
    while (std::getline(std::cin, test_expr))
    {
        if (test_expr.empty() || test_expr[0] == 'q' || test_expr[0] == 'Q')
            break;

		iterator_type iter = test_expr.begin();
        iterator_type end = test_expr.end();

        evalulater::error_handler<						// Our diagnostic printer
			iterator_type
		> error_handler(iter, end);						

		evalulater::parser<								// Our grammar
			iterator_type
		> eval(error_handler);

        evalulater::ast::expression ast;				// Our program (as AST)
		evalulater::vm::vmachine machine;				// Our virtual machine
		evalulater::compiler compiler(error_handler);	// Compiles the program
        boost::spirit::ascii::space_type space;
        bool r = phrase_parse(iter, end, eval, space, ast);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
			std::vector<evalulater::vm::byte_code> code = compiler.compile(ast);
            machine.execute(code);
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


