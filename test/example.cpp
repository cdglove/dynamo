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
 
    std::string str;
    while (std::getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

		evalulater::vm::vmachine machine;					// Our virtual machine
        std::vector<evalulater::vm::byte_code> code;	// Our VM code
        evalulater::parser<iterator_type> calc;			// Our grammar
        evalulater::ast::expression expression;			// Our program (AST)
        evalulater::compiler compile(code);				// Compiles the program

        iterator_type iter = str.begin();
        iterator_type end = str.end();
        boost::spirit::ascii::space_type space;
        bool r = phrase_parse(iter, end, calc, space, expression);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            compile(expression);
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


