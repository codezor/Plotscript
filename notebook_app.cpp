//#include"startup_config.hpp"
#include "interpreter.hpp"
#include "semantic_error.hpp"

#include "notebook_app.hpp"

// should run the application similair to plocript
NotebookApp::NotebookApp(QWidget* parent)
	: QWidget(parent)
{

	inputwidget = new InputWidget;
	outputwidget = new OutputWidget;

	inputwidget->setParent(parent);
	outputwidget->setParent(parent);
	

	auto layout = new QGridLayout();
	layout->addWidget(inputwidget, 0, 0);
	layout->addWidget(outputwidget, 1, 0);
	
	setLayout(layout);
	

}

//  REPL is a reapeated read-eval-print loop 
void repl()
{
	Interpreter interp;
}


// what are this 
void NotebookApp::plotScriptEvaluated(std::string output) {

}
// Keypress in the input widget has occurred
void NotebookApp::plotScriptInputReady(std::string  input) {

}