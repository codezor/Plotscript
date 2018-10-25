//#include"startup_config.hpp"
#include "interpreter.hpp"
#include "semantic_error.hpp"

#include "notebook_app.hpp"

// should run the application similair to plocript
NotebookApp::NotebookApp(QWidget* parent)
	: QWidget(parent)
{

	input = new InputWidget;
	output = new OutputWidget;

	input->setParent(parent);
	output->setParent(parent);
	
	
	// the user clicks shift+enter
	QObject::connect(input,SIGNAL(inputReady(QString)),this, SLOT(plotScriptInputReady(QString)));

	// When the plot script is evvaluate it will send back
	QObject::connect(this, SIGNAL(ExpressionReady(QString)), output, SLOT(DisplayItem(QString)));

	auto layout = new QGridLayout();
	layout->addWidget(input, 0, 0);
	layout->addWidget(output, 1, 0);
	
	setLayout(layout);
	

}
void NotebookApp::error(const std::string& err_str) {
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	
	outstream << "Error: " << err_str;
	out = outstream.str();
	TextforOut = QString::fromStdString(out);

	// Now Emmit the error signal 
	emit ExpressionReady(TextforOut);
}

void eval_from_stream(std::istream& stream) {
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	Interpreter interp;
	

	if (!interp.parseStream(stream)) {
		out = "Error:Invalid Program. Could not parse.";
		// Send a Parse error to output
		TextforOut = QString::fromStdString(out);
	}
	else {
		try {
			Expression exp = interp.evaluate();
			outstream<< "Error: " << exp;
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
		}
		catch (const SemanticError& ex) {
			outstream<< "Error: " << ex.what();
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
		}
	}
}

//  REPL is a reapeated read-eval-print loop 
void NotebookApp::repl(std::string line)
{

	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	Interpreter interp;
	//std::string line;
	std::istringstream expression(line);
	if (!interp.parseStream(expression)) {
		out = "Error: Invalid Expression. Could not parse.";
		// Send a Parse error to output
		TextforOut = QString::fromStdString(out);
	}
	else {
		try {
			Expression exp = interp.evaluate();
			outstream << "Error: " << exp;
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
		}
		catch (const SemanticError& ex) {
			outstream << "Error: " << ex.what();
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
		}
		// send thhs the exp to the output now
		// or catch and error
	}
	emit ExpressionReady(TextforOut);
}


// what are this 
//void NotebookApp::plotScriptEvaluated(QString output) {

//}
// Keypress in the input widget has occurred
void NotebookApp::plotScriptInputReady(QString input) {
		
	std::string line = input.toStdString();
	repl(line);
}