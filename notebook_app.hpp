/*! \ file notebook_app.hpp
 */
#ifndef NOTEBOOK_APP_HPP
#define NOTEBOOK_APP_HPP

#include <QWidget>
#include <QLayout>
#include <QDebug>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
// From the main program
#include"startup_config.hpp"
#include "interpreter.hpp"
#include "semantic_error.hpp"
// GUI Specific files
#include"input_widget.hpp"
#include"output_widget.hpp"
class NotebookApp : public QWidget
{
	Q_OBJECT

public:
	NotebookApp(QWidget* parent = nullptr);
	


private slots:
	// what are this 
	//void plotScriptEvaluated(QString output);
	// Keypress in the input widget has occurred
	void plotScriptInputReady(QString  input);

signals: 
	void ExpressionReady(QString Expression);

private:
	InputWidget* input;
	OutputWidget* output;
	Interpreter interp;
	//setchild
	void repl(std::string line);
	void error(const std::string& err_str);
	void startUp(Interpreter& interp);
	void eval_from_stream(std::istream& stream);
	void eval_from_file(std::string filename);
	void eval_from_command(std::string argexp);
};

#endif
