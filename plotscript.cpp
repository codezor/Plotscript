// This is an example of how to to trap Cntrl-C in a cross-platform manner
// it creates a simple REPL event loop and shows how to interrupt it.

#include <csignal>
#include <cstdlib>

// This global is needed for communication between the signal handler
// and the rest of the code. This atomic integer counts the number of times
// Cntl-C has been pressed by not reset by the REPL code.
volatile sig_atomic_t global_status_flag = 0;

// *****************************************************************************
// install a signal handler for Cntl-C on Windows
// *****************************************************************************
#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>

// this function is called when a signal is sent to the process
BOOL WINAPI interrupt_handler(DWORD fdwCtrlType)
{

	switch(fdwCtrlType)
	{
	case CTRL_C_EVENT: // handle Cnrtl-C
	  // if not reset since last call, exit
		if(global_status_flag > 0)
		{

			//exit(EXIT_FAILURE);
		}
		++global_status_flag;
		return TRUE;

	default:
		return FALSE;
	}
}

// install the signal handler
inline void install_handler()
{
	SetConsoleCtrlHandler(interrupt_handler, TRUE);
}
// *****************************************************************************

// *****************************************************************************
// install a signal handler for Cntl-C on Unix/Posix
// *****************************************************************************
#elif defined(__APPLE__) || defined(__linux) || defined(__unix) ||             \
    defined(__posix)
#include <unistd.h>

// this function is called when a signal is sent to the process
void interrupt_handler(int signal_num)
{

	if(signal_num == SIGINT)
	{ // handle Cn rtl-C
// if not reset since last call, exit
		if(global_status_flag > 0)
		{

			//exit(EXIT_FAILURE);
		}
		++global_status_flag;
	}
}

// install the signal handler
inline void install_handler()
{

	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = interrupt_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);
}
#endif
// *****************************************************************************

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <future>

#include"startup_config.hpp"
#include "interpreter.hpp"
#include "semantic_error.hpp"


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
	if(std::cin.fail() || std::cin.eof())
	{
		std::cin.clear(); // reset cin state
		line.clear(); //clear input string
		std::cout << "Error: interpreter kernel interrupted" << std::endl;
		line = "%reset";
	}
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

	// parsing and evaluating should occur in a seperate thread	
	if(!interp.parseStream(expression))
	{
		error("Error: Invalid Expression. Could not parse.");
	}
	else
	{
		try
		{
			Expression exp = interp.evaluate();
			std::cout << exp;
			//m_output.push(exp);
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

// A REPL is a repeated read-eval-print loop
// contains a parse and evaluate
void repl()
{
	
	Interpreter interp;
	startUp(interp);
	std::thread *kernalThread = new std::thread(&Interpreter::parseStreamQueue, &interp);//(plotscript_thread_main);	
	//bool interrupted = false;

	while(!std::cin.eof())
	{
		global_status_flag = 0;
		message_queue<OutMessage_t> &m_output = message_queue<OutMessage_t>::get_instance();
		message_queue<std::string> &m_input = message_queue<std::string>::get_instance();		
		
		if(!m_output.empty())
		{
			OutMessage_t results;
			m_output.wait_and_pop(results);
			if(results.type == OutMessage_t::Errorstring)
			{
				error(results.error);
				continue;
			}
			else if(results.type == OutMessage_t::noterr)
			{
				std::cout << results.exp << std::endl;
			}
			continue;
		}

		prompt();
		std::string line = readline();		

		if(line.empty()){
			continue;
		}
		
		
		if(line == "%stop")
		{
			if(kernalThread != nullptr)
			{
				//0exitSignal.set_value(false);
				//kernalThread->join();
				m_input.push(line);
				while(!m_input.empty())
				{

				}
				kernalThread->join();
				delete kernalThread;
				//kernalThread->~thread();
				kernalThread = nullptr;
				//is_thread_alive = false;
				continue;
			}

		}

		else if(line == "%reset")
		{
			if(kernalThread != nullptr)
			{
				m_input.push("%stop");
				while(!m_input.empty())
				{

				}
				kernalThread->join();
				//kernalThread->detach();

					//ExitProccess				
				delete kernalThread;
				kernalThread = nullptr;
				interp.clearInterp();
				startUp(interp);
				kernalThread = new std::thread(&Interpreter::parseStreamQueue, &interp);
				continue;

			}
		}
		else if(line == "%start")
		{
			if(kernalThread == nullptr)
			{
				kernalThread = new std::thread(&Interpreter::parseStreamQueue, &interp);
			}
			continue;
		}
		else if(line == "%exit")
		{
			m_input.push("%stop");
			while(!m_input.empty())
			{
			}
			kernalThread->join();
			//ExitProccess				
			delete kernalThread;
			kernalThread = nullptr;
			return;
		}
		else
		{
			if(kernalThread == nullptr)
			{
				error("Interpreter kernel not running");
				continue;
			}
			m_input.push(line);			
			while(m_output.empty())
			{
				if(global_status_flag > 0)
				{					
					break;
				}
			}		
			
			continue;
		}
	}
}

int
main(int argc, char* argv[])
{
	//std::thread MainThread = std::thread(repl );
	install_handler();
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
			error("Error: Incorrect number of command line arguments.");
		}

	}
		
	repl();
	return EXIT_SUCCESS;
}