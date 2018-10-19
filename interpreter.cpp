#include "interpreter.hpp"

// system includes
#include <stdexcept>

// module includes
#include "environment.hpp"
#include "expression.hpp"
#include "parse.hpp"
#include "semantic_error.hpp"
#include "token.hpp"
//#include "startup_config.hpp"

bool
Interpreter::parseStream(std::istream& expression) noexcept
{

  TokenSequenceType tokens = tokenize(expression);

  ast = parse(tokens);

  return (ast != Expression());
};

Expression
Interpreter::evaluate()
{

  return ast.eval(env);
}
