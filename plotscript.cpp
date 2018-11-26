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


//typedef message_queue<std::string>  errorq;
//typedef message_queue<std::string>  input;
//typedef message_queue<Expression>  output;

/*typedef int Message;
typedef message_queue<Message> MessageQueue;
class Consumer
{
public:
	// Looks like a  function call but it only produces this one thing
	Consumer(MessageQueue* messageQueuePtr, int identifier)
	{
		mq = messageQueuePtr;
		id = identifier;
	}
	void operator()() const
	{
		for(int i = 0; i < 10; ++i)
		{
			Message m;
			mq->wait_and_pop(m);
			int work_time = unif(gen); // generate random work time
			std::cout << "Consumer " << id << " has taken product " << m
				<< " will work for " << work_time << " ms." << std::endl;
			std::this_thread::sleep_for(
				std::chrono::milliseconds(work_time)); // random sleep time
		}
	}
private:
	int id;
	MessageQueue* mq;
};
class Producer
{
public:
	Producer(MessageQueue* messageQueuePtr)
	{
		mq = messageQueuePtr;
	}
	void operator()() const
	{
		for(int i = 0; i < 10; ++i)
		{
			mq->push(i);
			std::cout << "Producer has added product " << i << std::endl;
			int work_time = unif(gen);
			std::this_thread::sleep_for(std::chrono::milliseconds(work_time));
		}
	}
private:
	MessageQueue* mq;
};*/

void
prompt()
{
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

void EvalOne(Interpreter& interp, std::istringstream& expression)
{
	//message_queue<std::string>  input;
	message_queue<Expression> &output= message_queue<Expression>::get_instance();
	// parsing and evaluating should occur in a seperate thread	
	if(!interp.parseStream(expression))
	{
		//message_queue<std::string> & errorq = message_queue<std::string>::get_instance();
		//errorq.push("Error: Invalid Expression. Could not parse.");
		error("Invalid Expression. Could not parse.");

	}
	else
	{
		try
		{
			Expression exp = interp.evaluate();
			//std::cout << exp;
			output.push(exp);

		}
		catch(const SemanticError& ex)
		{
			std::cerr << ex.what() << std::endl;

		}
	}
}

// contains parse and evaluate
void startUp(Interpreter& interp)
{

	std::ifstream ifs(STARTUP_FILE);
	//EvalOne(interp, ifs);
	if(!interp.parseStream(ifs))
	{
		error("Invalid Program. Could not parse.");
		//return EXIT_FAILURE;
	}
	else
	{
		try
		{
			Expression exp = interp.evaluate();
			//std::cout << exp << std::endl;
		}
		catch(const SemanticError& ex)
		{
			std::cerr << ex.what() << std::endl;
			//return EXIT_FAILURE;
		}
	}
}

int
eval_from_stream(std::istream& stream, Interpreter& interp)
{
	// Parse 
	if(!interp.parseStream(stream))
	{
		error("Invalid Program. Could not parse.");
		return EXIT_FAILURE;
	}
	else
	{
		try
		{
			// Evaluate
			Expression exp = interp.evaluate();
			std::cout << exp << std::endl;
		}
		catch(const SemanticError& ex)
		{
			std::cerr << ex.what() << std::endl;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int
eval_from_file(std::string filename)
{
	Interpreter interp;
	startUp(interp);
	std::ifstream ifs(filename);

	if(!ifs)
	{
		error("Could not open file for reading.");
		return EXIT_FAILURE;
	}
	return eval_from_stream(ifs, interp);
}

int
eval_from_command(std::string argexp)
{
	Interpreter interp;
	startUp(interp);
	std::istringstream expression(argexp);

	return eval_from_stream(expression, interp);
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


void plotscript_thread_main()
{
	message_queue<std::string> &input = message_queue<std::string>::get_instance();
	//message_queue<Expression> & output = message_queue<Expression>::get_instance();
	message_queue<std::string>  &errorq = message_queue<std::string>::get_instance();

	Interpreter interp;
	startUp(interp);

	while(true)
	{

		if(errorq.empty())
		{

			//prompt();
			std::string line;
			input.wait_and_pop(line);

			if(line.empty())
				continue;

			std::istringstream expression(line);
			EvalOne(interp, expression);
		}

	}
}


// A REPL is a repeated read-eval-print loop
// contains a parse and evaluate
void
repl()
{
	std::thread *kernalThread = nullptr;//(plotscript_thread_main);
	//bool is_thread_alive = true;
	message_queue<std::string> &input = message_queue<std::string>::get_instance();
	message_queue<Expression> & output = message_queue<Expression>::get_instance();
	message_queue<std::string>  &errorq = message_queue<std::string>::get_instance();
	while(!std::cin.eof())
	{


		if(!output.empty())
		{
			Expression results;
			output.wait_and_pop(results);
			std::cout << results << std::endl;
			continue;
		}

		prompt();
		std::string line = readline();
		if(line.empty())
			continue;
		//else 
		if(line == "%stop")
		{
			if(kernalThread != nullptr)
			{

				kernalThread->detach();
				delete kernalThread;
				kernalThread = nullptr;
				//is_thread_alive = false;
				continue;
			}

		}
		else if(line == "%reset")
		{
			if(kernalThread != nullptr)
			{
				kernalThread->detach();
				delete kernalThread;
				kernalThread = nullptr;
				kernalThread = new std::thread(plotscript_thread_main);
				continue;

			}
		}
		else if(line == "%start")
		{

			if(kernalThread == nullptr)
			{
				kernalThread = new std::thread(plotscript_thread_main);

			}

			continue;
		}
		else
		{
			//std::istringstream expression(line);
			//EvalOne(interp, expression);
			if(kernalThread == nullptr)
			{
				error("Interpreter kernel not running");
				continue;
			}
			else
			{

				input.push(line);

				while(output.empty())
				{
					//if(!errorq.empty())
					//{

						//std::string  er;
						//errorq.wait_and_pop(er);
						//break;
					//}

				}
				continue;
			}

		}

	}

}



int
main(int argc, char* argv[])
{
	std::thread *MainThread;
	MainThread = new std::thread( repl );
	MainThread->join();
	return 0;

	if(argc == 2)
	{

		// this will parse and evaluate
		return eval_from_file(argv[1]);

	}

	else if(argc == 3)
	{


		if(std::string(argv[1]) == "-e")
		{
			// this will parse and evaluate 
			return eval_from_command(argv[2]);
		}
		else
		{
			error("Incorrect number of command line arguments.");
		}

	}

	/*else
	{
		repl();
	}*/

	return EXIT_SUCCESS;
}