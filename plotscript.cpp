#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>

#include"startup_config.hpp"
#include "interpreter.hpp"
#include "semantic_error.hpp"
#include "message_queue.hpp"
void
prompt(){	
  std::cout << "\nplotscript> ";
}

std::string
readline()
{
  std::string line;
  std::getline(std::cin, line);

  return line;
}

void
error(const std::string& err_str)
{
  std::cerr << "Error: " << err_str << std::endl;
}

void
info(const std::string& err_str)
{
  std::cout << "Info: " << err_str << std::endl;
}

// contains parse and evaluate
void startUp(Interpreter& interp) {

	std::ifstream ifs(STARTUP_FILE);
	if (!interp.parseStream(ifs)) {
		error("Invalid Program. Could not parse.");
		//return EXIT_FAILURE;
	}
	else {
		try {
			Expression exp = interp.evaluate();
			//std::cout << exp << std::endl;
		}
		catch (const SemanticError& ex) {
			std::cerr << ex.what() << std::endl;
			// return EXIT_FAILURE;
		}
	}
}

int
eval_from_stream(std::istream& stream)
{
	Interpreter interp;
	startUp(interp);
	// Parse 
  if (!interp.parseStream(stream)) {
    error("Invalid Program. Could not parse.");
    return EXIT_FAILURE;
  } else {
    try {
		// Evaluate
      Expression exp = interp.evaluate();
      std::cout << exp << std::endl;
    } catch (const SemanticError& ex) {
      std::cerr << ex.what() << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int
eval_from_file(std::string filename)
{
  std::ifstream ifs(filename);

  if (!ifs) {
    error("Could not open file for reading.");
    return EXIT_FAILURE;
  }
  return eval_from_stream(ifs);
}

int
eval_from_command(std::string argexp)
{
	//eval_from_file(STARTUP_FILE);
  std::istringstream expression(argexp);

  return eval_from_stream(expression);
}

// Im sure I shouln't do this 
int eval_from_message_queue(message_queue<std::string> &queue)
{
	while(!queue.empty())
	{
		std::string string;// = queue.front();
		//std::string const string = 
		queue.wait_and_pop(string);
		// Eval from command uses eval from stream. Eval from stream instantiates a new interpereter.
		int const result = eval_from_command(string);
		if(result != EXIT_SUCCESS)
		{
			return result;
		}
		//queue.pop();
	}

	return EXIT_SUCCESS;
}

// A REPL is a repeated read-eval-print loop
// contains a parse and evaluate
void
repl()
{
  Interpreter interp;   
  startUp(interp);
  //message_queue<std::string> & plotscript_program_queue = message_queue<std::string>::get_instance();
  
  while (!std::cin.eof()) {
	
    prompt();
    std::string line = readline();	
  
	if (line.empty())
      continue;
	
    std::istringstream expression(line);

	// parsing and evaluating should occur in a seperate thread
	
    if (!interp.parseStream(expression)) {
      error("Invalid Expression. Could not parse.");
    } else {
      try {
		  // interpret should occur in the same thread as parsing
		
        Expression exp = interp.evaluate();
        std::cout << exp << std::endl;
      } catch (const SemanticError& ex) {
        std::cerr << ex.what() << std::endl;
      }
    }
  }
}

void plotscript_thread_main(message_queue<std::string> &queue)
{
	//message_queue<std::string> & queue = message_queue<std::string>::get_instance();
	Interpreter interp;
	startUp(interp);
	//while(true)
	//{
		//std::string plotscript_program;
		//queue.wait_and_pop(plotscript_program);

		while(!std::cin.eof())
		{

			prompt();
			std::string line = readline();
			queue.push(line);

			if(line.empty())
				continue;
		
			std::istringstream expression(line);

			// parsing and evaluating should occur in a seperate thread
			if(!interp.parseStream(expression))
			{
				error("Invalid Expression. Could not parse.");
			}
			else
			{
				try
				{
					// interpret should occur in the same thread as parsing

					Expression exp = interp.evaluate();
					std::cout << exp << std::endl;
				}
				catch(const SemanticError& ex)
				{
					std::cerr << ex.what() << std::endl;
				}
			}
			//std::cout << "In da threadz" << std::endl;
		}
	

	
}

int
main(int argc, char* argv[])
{
	message_queue<std::string> & plotscript_program_queue = message_queue<std::string>::get_instance();
	//std::string test_program = "(+ 1 2)";
	//plotscript_program_queue.push(test_program);
	//eval_from_message_queue(plotscript_program_queue, interp);
	//std::thread t1(eval_from_message_queue( plotscript_program_queue, interp));
	//t1.join();
	//return 0;
	std::thread plotscript_thread(plotscript_thread_main, std::ref(plotscript_program_queue));
	plotscript_thread.join();
	return 0;
  if (argc == 2) {
	 
	  // this will parse and evaluate
    return eval_from_file(argv[1]);
  
  } 
 
  else if (argc == 3) {
	  	  

    if (std::string(argv[1]) == "-e") {
		// this will parse and evaluate 
      return eval_from_command(argv[2]);
    } 
	else {
      error("Incorrect number of command line arguments.");
    }  
	 
  } 
 
  else {	
  
	  // this will parse and evaluate  
    repl();
	
  }
 
  return EXIT_SUCCESS;
}
