#include "interpreter.hpp"

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
	message_queue<std::string> &m_input = message_queue<std::string>::get_instance();
	while(true)
	{
		if(!m_input.empty())
		{
			std::string line;
			m_input.wait_and_pop(line);
			if(line =="%stop")
			{
				break;
			}
			
			std::istringstream exps(line);

			TokenSequenceType tokens = tokenize(exps);

			ast = parse(tokens);
			message_queue<Expression> &m_output = message_queue<Expression>::get_instance();

			m_output.push(ast.eval(env));
		}
	}
	//return false;
	return;
}
// evaluates the ast enviornment - output
Expression Interpreter::evaluate(){
	// eval consumes the AST
	
  return ast.eval(env);
}
void Interpreter::clearInterp()
{
	env.reset();
}
