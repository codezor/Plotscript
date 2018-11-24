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

  return (ast != Expression());
};

// evaluates the ast enviornment - output
Expression Interpreter::evaluate(){
	// eval consumes the AST
  return ast.eval(env);
}
