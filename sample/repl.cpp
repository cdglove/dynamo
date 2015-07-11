// ****************************************************************************
// dynamo/test/example.cpp
//
// Sample program for dynamo expression eveluator.
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
#include "dynamo/compiler.hpp"
#include "dynamo/linker.hpp"

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

	dynamo::constant_index extern_state;
	float t1 = 5.f;
	float t2 = 11.f;
	extern_state["t1"] = load_float_ptr(&t1);
	extern_state["t2"] = load_float_ptr(&t2);

	dynamo::variable_index local_state;
	dynamo::vm::machine machine;				    // Our virtual machine -- reusable
	dynamo::diagnostic_sink diagnostic(std::cout);  // Out diagnostic printer -- reusable
	
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

		dynamo::parse::string_parser parser(diagnostic);	// Builds the AST
		dynamo::compiler compiler(diagnostic);				// Compiles the program
		dynamo::linker linker(diagnostic);					// Links the program
        
		boost::optional<
			dynamo::ast::statement_list
		> ast = parser.parse(test_expr);
		     
        if(ast)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";

			boost::optional<
				dynamo::vm::byte_code
			> code = compiler.compile(*ast, parser.get_indexed_source());

			if(code)
			{
				std::cout << "\nCompiling succeeded\n";
				
				boost::optional<
					dynamo::vm::executable
				> exe = linker.link(*code, extern_state, local_state);	
				
				if(exe)
				{
					std::cout << "\nLinking succeeded\n";

					machine.execute(*exe);

					std::cout << "\nExecution succeeded\n";
					if(machine.has_result())
					{
						std::cout << "\nResult: " << machine.top() << "\n";
					}
					else
					{
						std::cout << "\nNo result (stack clear)" << "\n";
					}
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
            std::cout << "\nParsing failed\n";
        }
		
		std::cout << "-------------------------" << std::endl;
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}


