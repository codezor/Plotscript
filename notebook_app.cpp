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
	
	// connect (object1, signal_1, Object2, slot1)
	// connect (object1, signal_1, object2, slot2)
	// Connect the Input to the output for the output text to update when 
	// the user clicks shift+enter
	QObject::connect(inputwidget,SIGNAL(inputReady(QString)),outputwidget, SLOT(DisplayItem(QString)));

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
void NotebookApp::plotScriptEvaluated(QString output) {

}
// Keypress in the input widget has occurred
void NotebookApp::plotScriptInputReady(QString input) {

	
}