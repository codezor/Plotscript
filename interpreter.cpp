#include "interpreter.hpp"
//#include <iostream>
// system includes
#include <stdexcept>

// module includes
#include "environment.hpp"
#include "expression.hpp"
#include "parse.hpp"
#include "semantic_error.hpp"
#include "token.hpp"


// returns a bool but parses the information into the ast - input
bool Interpreter::parseStream(std::istream& expression) noexcept
{
	TokenSequenceType tokens = tokenize(expression);

	ast = parse(tokens);
	return ( ast != Expression() );
}

void Interpreter::parseStreamQueue()
{

	while(true)
	{
		message_queue<std::string> &m_input = message_queue<std::string>::get_instance();
		message_queue<OutMessage_t> &m_output = message_queue<OutMessage_t>::get_instance();
		if(!m_input.empty())
		{
			std::string line;
			m_input.wait_and_pop(line);
			if(line == "%stop")
			{
				break;
			}
			auto temp_ast = ast;
			auto temp_env = env;
			std::istringstream expression(line);
			if(!parseStream(expression))
			{
			//	error();
				OutMessage_t out;
				out.error = "Error: Invalid Expression. Could not parse.";
				out.type = OutMessage_t::Errorstring;
				m_output.push(out);
				continue;
			}

			else
			{
				
				try
				{	
					
					evaluate();
					OutMessage_t out;
					out.exp = ast.eval(env);
					out.type = OutMessage_t::noterr;
					m_output.push(out);
					continue;

				}
				catch(const SemanticError& ex)
				{
					OutMessage_t out;
					
					out.error = ex.what();
					if(out.error == "Error: interpreter kernel interrupted")
					{
						ast = temp_ast;
						env = temp_env;
					}
					out.type = OutMessage_t::Errorstring;
					m_output.push(out);
					
					continue;
				}	
				

			}
		}
	}
	
}


Expression Interpreter::evaluate(){
	// eval consumes the AST	
	
	return  ast.eval(env);;
		
}
void Interpreter::clearInterp()
{
	env.reset();
}


