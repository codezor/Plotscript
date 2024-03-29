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
			exit(EXIT_FAILURE);
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
	{ // handle Cnrtl-C
// if not reset since last call, exit
		if(global_status_flag > 0)
		{
			exit(EXIT_FAILURE);
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
#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <chrono>    
#include "notebook_app.hpp"

//#include "message_queue.hpp"
// should run the application similair to plocript
NotebookApp::NotebookApp(QWidget* parent)
	: QWidget(parent)
{
	m_plotscript_thread_ptr = new std::thread(&Interpreter::parseStreamQueue, &interp);
	install_handler();
	input = new InputWidget;
	output = new OutputWidget;
	input->setObjectName("input");
	output->setObjectName("output");
	
	input->setParent(parent);
	output->setParent(parent);

	// Buttons 
	startButton = new QPushButton("Start Kernel", parent);
	startButton->setObjectName("start");
	
	stopButton = new QPushButton("Stop Kernel", parent);
	stopButton->setObjectName("stop");

	resetButton = new QPushButton("Reset Kernel", parent);
	resetButton->setObjectName("reset");

	interruptButton = new QPushButton("Interrupt", parent);
	interruptButton->setObjectName("interrupt");	

	layout = new QGridLayout;
	layout->addWidget(startButton, 0, 0);
	layout->addWidget(stopButton, 0, 1);
	layout->addWidget(resetButton, 0, 2);
	layout->addWidget(interruptButton, 0, 3);
	layout->addWidget(input, 1, 0, 1, 4);
	layout->addWidget(output, 2, 0, 1, 4);
	
	setLayout(layout);

	//Start Up file should be called before input is avaliable	
	startUp(interp);
	
	// The user clicks shift+enter
	QObject::connect(input,SIGNAL(inputReady(QString)),this, SLOT(plotScriptInputReady(QString)));

	// When the plot script is evaluate it will send back
	QObject::connect(this, SIGNAL(ExpressionReady(QString)), output, SLOT(DisplayItem(QString)));

	// Connecting elipse signal to Elipse output
	QObject::connect(this, SIGNAL(EllipseReady(double, double, double)), output, SLOT(DisplayPoint(double, double, double)));

	// Conect the line
	QObject::connect(this, SIGNAL(LineReady(double, double, double, double , double )), output, SLOT(DisplayLine(double, double, double, double, double)));

	// Draw the text
	QObject::connect(this, SIGNAL(TextReady(QString, double , double, double, double)), output, SLOT(DisplayText(QString, double, double, double, double)));

	// Draw the Discrete plot
	QObject::connect(this, SIGNAL(discretePlotReady(QString , QString , QString , double , double , double , double, double )), output, SLOT(DisplayDiscretePlot(QString, QString, QString, double, double, double, double, double)));

	// Engable the input widget once the output widget is displaying 
	QObject::connect(this, SIGNAL(OutputWidgetDisplayed()), this, SLOT(EnableInputWidget()));

	// Clear the display
	QObject::connect(this, SIGNAL(ClearScene()), output, SLOT(DisplayClear()));
	QObject::connect(stopButton, SIGNAL(released()), this, SLOT(stopButtonPressed()));
	QObject::connect(startButton, SIGNAL(released()), this, SLOT(startButtonPressed()));
	QObject::connect(resetButton, SIGNAL(released()), this, SLOT(resetButtonPressed()));
	QObject::connect(interruptButton, SIGNAL(released()), this, SLOT(interruptButtonPressed()));

}

void NotebookApp::error(const std::string& err_str) {
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	
	outstream << err_str;
	out = outstream.str();
	TextforOut = QString::fromStdString(out);

	///Now Emmit the error signal 
	emit ExpressionReady(TextforOut);
	//std::cerr << "Error: " << err_str << std::endl;
}

void NotebookApp::info(const std::string& err_str)
{
	
	std::stringstream outstream;
	std::string out;
	QString TextforOut;

	outstream << "Info: " << err_str;
	out = outstream.str();
	TextforOut = QString::fromStdString(out);

	// Now Emmit the error signal 
	emit ExpressionReady(TextforOut);
}

// contains both parse and evaluate which will need to be done in a seperate thread
void NotebookApp::startUp(Interpreter& interp) {
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	
	std::ifstream ifs(STARTUP_FILE);

	if (!interp.parseStream(ifs)) {
		out = "Error: Invalid Expression. Could not parse.";
		// Send a Parse error to output
		TextforOut = QString::fromStdString(out);
		
	}
	else {
		try {
			Expression exp = interp.evaluate();
			outstream << exp;
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
		}
		catch (const SemanticError& ex) {
			outstream << ex.what() << std::endl;
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
			// return EXIT_FAILURE;
		}
	}
	//emit ExpressionReady(TextforOut);
}
// contains both parse and evaluate which will need to be done in a seperate thread
void NotebookApp::eval_from_stream(std::istream& stream) {
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	Interpreter interp;
	

	if (!interp.parseStream(stream)) {
		out = "Error: Invalid Expression. Could not parse.";
		// Send a Parse error to output
		TextforOut = QString::fromStdString(out);
		
	}
	else {
		try {
			Expression exp = interp.evaluate();
			outstream<< exp;
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
		}
		catch (const SemanticError& ex) {
			outstream<< "Error: " << ex.what();
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
			//std::cerr << ex.what() << std::endl;
		}
	}
	emit ExpressionReady(TextforOut);
}
// contains both parse and evaluate which will need to be done in a seperate thread
void NotebookApp::eval_from_file(std::string filename) {
	std::ifstream ifs(filename);
	
	std::stringstream outstream;
	std::string out;
	QString TextforOut;

	if (!ifs) {

		out = "Error: Invalid Expression. Could not parse.";
		// Send a Parse error to output
		TextforOut = QString::fromStdString(out);
		emit ExpressionReady(TextforOut);
	}

	eval_from_stream(ifs);
}

void NotebookApp::eval_from_command(std::string argexp) {
	
	std::istringstream expression(argexp);
	eval_from_stream(expression);
}

//  REPL is a reapeated read-eval-print loop 
// contains both parse and evaluate which will need to be done in a seperate thread
void NotebookApp::repl(std::string line) //TODO: rename since this technically isn't a loop now
{
	// interp;
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	
	message_queue<std::string> &m_input = message_queue<std::string>::get_instance();

	std::istringstream expression(line);

	if(line == "%interrupt")
	{
		global_status_flag = 0;
		if(m_plotscript_thread_ptr != nullptr)
		{			
			m_input.push("%stop");
			emit(ClearScene());
			emit ExpressionReady("Error: interpreter kernel interrupted");
			
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			m_plotscript_thread_ptr->join();
						
			delete m_plotscript_thread_ptr;
			m_plotscript_thread_ptr = nullptr;
			interp.clearInterp();
			startUp(interp);
			m_plotscript_thread_ptr = new std::thread(&Interpreter::parseStreamQueue, &interp);
			
		}
		emit OutputWidgetDisplayed();
	}
	
	else if (line == "%stop")
		{
			if(m_plotscript_thread_ptr != nullptr)
			{
				m_input.push(line);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				m_plotscript_thread_ptr->join();
				delete m_plotscript_thread_ptr;
				
				m_plotscript_thread_ptr = nullptr;
				
			}
			emit OutputWidgetDisplayed();
		}
		else if(line == "%reset")
		{
			if(m_plotscript_thread_ptr != nullptr)
			{
				m_input.push("%stop");
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				m_plotscript_thread_ptr->join();
				//ExitProccess				
				delete m_plotscript_thread_ptr;
				m_plotscript_thread_ptr = nullptr;
				interp.clearInterp();
				startUp(interp);
				m_plotscript_thread_ptr = new std::thread(&Interpreter::parseStreamQueue, &interp);
				
			}
			emit OutputWidgetDisplayed();
		}
		else if(line == "%start")
		{

			if(m_plotscript_thread_ptr == nullptr)
			{
				m_plotscript_thread_ptr = new std::thread(&Interpreter::parseStreamQueue, &interp);
			}
			emit OutputWidgetDisplayed();
			
		}
		else
		{			
			if(m_plotscript_thread_ptr == nullptr)
			{
				
			}
			else
			{
				input->setReadOnly(true);
				m_input.push(line);
			}			
		}
	
}

void NotebookApp::stopButtonPressed()
{
	repl("%stop");
}

void NotebookApp::startButtonPressed()
{
	repl("%start");
}

void NotebookApp::resetButtonPressed()
{	
	repl("%reset");
}
void NotebookApp::interruptButtonPressed()
{
		
	signal(SIGINT, 0);
	repl("%interrupt");
	
}
void NotebookApp::plotScriptInputReady(QString InputText) {
	
	// Turn input to string	
	std::string line = InputText.toStdString();	
	emit(ClearScene());
	repl(line);
	outputPolling();
		
}
void NotebookApp::outputPolling()
{

	while(true){
		
		if(m_plotscript_thread_ptr == nullptr)
		{
			emit ExpressionReady("Error: Interpreter kernel not running");
			emit OutputWidgetDisplayed();
			break; //continue;
		}

		if(global_status_flag > 0)
		{
			break;
		}

		message_queue<OutMessage_t> &m_output = message_queue<OutMessage_t>::get_instance();
		
		Expression exp;
		if(!m_output.empty())
		{
			OutMessage_t results;
			m_output.try_pop(results);			
			
			if(results.type == OutMessage_t::Errorstring)
			{

				QString TextforOut;
				TextforOut = QString::fromStdString(results.error);

				emit ExpressionReady(TextforOut);	
				emit OutputWidgetDisplayed();
				break;
			}
			else if(results.type == OutMessage_t::noterr)
			{
				Expression exp = exp;
				whatGoesWhere(results.exp);				
				break;
				
			}	
			continue;
		}
		
		
	}
	
}

void NotebookApp::whatGoesWhere(Expression exp) {	
	
		if(!exp.isPropertyListEmpty())
		{
			std::string propertyKey = "\"object-name\"";
			Expression objectName;
			objectName = exp.getPropertyList(propertyKey);
			if(objectName.isHeadString())
			{
				// Draw point
				if(objectName.head().asString() == "\"point\"")
				{
					makePoint(exp);
				}
				// Draw line
				if(objectName.head().asString() == "\"line\"")
				{
					makeLine(exp);
				}
				//DrawText
				if(objectName.head().asString() == "\"text\"")
				{
					makeText(exp);
				}
				// Draw Plot discrete-plot
				if(objectName.head().asString() == "\"discrete-plot\"" || objectName.head().asString() == "\"continuous-plot\"")
				{
					//
					makeDiscretePlot(exp);
				}
			}
		}
		else if(exp.isHeadList())
		{
			for(auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e)
			{
				whatGoesWhere(*e);
			}
		}

		// Regular plot script things 
		else
		{
			makeExpression(exp);
		}
	
}

void NotebookApp::makeExpression(Expression exp) {
	
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	if (!exp.isHeadLambda() || !exp.isHeadSymbol()) {
		outstream << exp;
		out = outstream.str();
		TextforOut = QString::fromStdString(out);
		emit ExpressionReady(TextforOut);
		emit OutputWidgetDisplayed();
	}
}

void NotebookApp::makeLine(Expression exp){

	Expression lineThickness;
	lineThickness = exp.getPropertyList("\"thickness\"");
	if (lineThickness.head().asNumber()<0) {
		// emit the signal for the error message
	}
	auto point1 = exp.tailConstBegin();
	auto point2 = exp.tailConstEnd();
	std::vector<Expression> line(point1, point2);

	auto p1x = line[0].tailConstBegin();
	auto p1y = line[0].tailConstEnd();
	auto p2x = line[1].tailConstBegin();
	auto p2y = line[1].tailConstEnd();

	std::vector<Expression> p1(p1x, p1y);
	std::vector<Expression> p2(p2x, p2y);

	emit(LineReady(p1[0].head().asNumber(), p1[1].head().asNumber(), p2[0].head().asNumber(), 
		p2[1].head().asNumber(), lineThickness.head().asNumber()));
	emit OutputWidgetDisplayed();
}

void NotebookApp::makePoint(Expression exp){
	
	Expression pointSize;
	//Expression 
	pointSize = exp.getPropertyList("\"size\"");
	if (pointSize.head().asNumber() < 0) {
		// emit the signal for the error message
	}
	auto xcor = exp.tailConstBegin();
	auto ycor = exp.tailConstEnd();
	std::vector<Expression> cordinates(xcor, ycor);

	Expression x = cordinates.front();
	Expression y = cordinates.back();

	emit(EllipseReady(x.head().asNumber(), y.head().asNumber(), pointSize.head().asNumber()));
	emit OutputWidgetDisplayed();
}

void NotebookApp::makeText(Expression exp){
	
	Expression textPosition;
	textPosition = exp.getPropertyList("\"position\"");	

	Expression textScale;
	textScale = exp.getPropertyList("\"text-scale\"");	
	
	Expression textRotation;
	textRotation = exp.getPropertyList("\"text-rotation\"");	
	
	auto xcor = textPosition.tailConstBegin();
	auto ycor = textPosition.tailConstEnd();
	std::vector<Expression> cordinates(xcor, ycor);
	QString words = QString::fromStdString(exp.head().asString());

	textPosition = exp.getPropertyList("\"position\"");
	//textPosition = exp.getPropertyList("\"position\"");
	emit(TextReady(words, cordinates[0].head().asNumber(), cordinates[1].head().asNumber(), textRotation.head().asNumber(), textScale.head().asNumber()));
	emit OutputWidgetDisplayed();
}

void NotebookApp::makeDiscretePlot(Expression exp) {

	// discreete props list

	
	for(auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e)
	{
		Expression express = *e;
		std::string propertyKey = "\"object-name\"";
		Expression objectName;
		objectName = express.getPropertyList(propertyKey);
		// Draw point
		if(objectName.head().asString() == "\"point\"")
		{
			makePoint(*e);
		}
		// Draw line
		if(objectName.head().asString() == "\"line\"")
		{
			makeLine(*e);
		}
		//DrawText
		if(objectName.head().asString() == "\"text\"")
		{
			makeText(*e);
		}
		else
		{
			//whatGoesWhere(*e);
		}
	}

	Expression title;
	title = exp.getPropertyList("\"title\"");

	Expression xlable;
	xlable = exp.getPropertyList("\"abscissa-label\"");

	Expression ylable;
	ylable = exp.getPropertyList("\"ordinate-label\"");

	Expression textScale;
	textScale = exp.getPropertyList("\"text-scale\"");
	if(!textScale.isHeadNumber())
	{
		textScale = Expression(Atom(1.0));
	}
	Expression ymin;
	ymin = exp.getPropertyList("\"ymin\"");

	Expression ymax;
	ymax = exp.getPropertyList("\"ymax\"");

	Expression xmin;
	xmin = exp.getPropertyList("\"xmin\"");
	
	Expression xmax;
	xmax = exp.getPropertyList("\"xmax\"");

	QString Qtitle = QString::fromStdString(title.head().asString());
	QString Qxlable = QString::fromStdString(xlable.head().asString());
	QString Qylabel = QString::fromStdString(ylable.head().asString());

	//textScale = exp.getPropertyList("\"text-scale\"");
	emit(discretePlotReady(Qtitle, Qxlable, Qylabel ,xmin.head().asNumber(), xmax.head().asNumber(), ymin.head().asNumber(), ymax.head().asNumber(), textScale.head().asNumber()));
	emit OutputWidgetDisplayed();

}

void NotebookApp::EnableInputWidget()
{

	input->setReadOnly(false);
}

NotebookApp::~NotebookApp()
{
	if(m_plotscript_thread_ptr != nullptr)
	{
		message_queue<std::string> &m_input = message_queue<std::string>::get_instance();
		m_input.push("%stop");
		while(!m_input.empty())
		{

		}
		m_plotscript_thread_ptr->join();
		//ExitProccess				
		delete m_plotscript_thread_ptr;
		m_plotscript_thread_ptr = nullptr;
	}
}