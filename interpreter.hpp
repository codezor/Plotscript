/*! \file interpreter.hpp

The interpreter can parse from a stream into an internal AST and evaluate it.
It maintains an environment during evaluation.
 */

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

// system includes
#include <istream>
#include <string>

// threading 
#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <future>

// Signal 
#include <csignal>
#include <cstdlib>

// module includes
#include "environment.hpp"
#include "expression.hpp"
#include "message_queue.hpp"


//union OutMessage_t
//{
	//std::string error;
	//Expression exp;
//}OutMessage;
/*! \class Interpreter
\brief Class to parse and evaluate an expression (program)

Interpreter has an Environment, which starts at a default.
The parse method builds an internal AST.
The eval method updates Environment and returns last result.
*/
class Interpreter
{
public:
  /*! Parse into an internal Expression from a stream
    \param expression the raw text stream repreenting the candidate expression
    \return true on successful parsing
   */
  bool parseStream(std::istream& expression) noexcept;
 // bool parseStream() noexcept;
  void parseStreamQueue();
  /*! Evaluate the Expression by walking the tree, returning the result.
    \return the Expression resulting from the evaluation in the current
    environment \throws SemanticError when a semantic error is encountered
   */
  Expression evaluate();
  void clearInterp();
  /*void interrupt_handler(int signal_num)
  {

	  if(signal_num == SIGINT)
	  { // handle Cn rtl-C
  // if not reset since last call, exit
		  if(global_status_flag > 0)
		  {
			  exit(EXIT_FAILURE);
		  }
		  ++global_status_flag;
	  }
  }*/
private:
  // the environment
  Environment env;

  // the AST
  Expression ast;
};

#endif
