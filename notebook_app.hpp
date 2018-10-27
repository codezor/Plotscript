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
	
	//void plotScriptEvaluated(QString output);
	// Keypress in the input widget has occurred
	void plotScriptInputReady(QString  input);

signals: 
	void ExpressionReady(QString Expression);
	void EllipseReady(double x, double  y, double size);
	void LineReady(double x1, double y1, double x2, double y2, double thickness);
	void TextReady(QString write, double x, double y);

private:
	InputWidget* input;
	OutputWidget* output;
	Interpreter interp;
	//setchild
	void repl(std::string line);
	void error(const std::string& err_str);
	void info(const std::string& err_str);

	void startUp(Interpreter& interp);
	void eval_from_stream(std::istream& stream);
	void eval_from_file(std::string filename);
	void eval_from_command(std::string argexp);
	//QString 
	void whatGoesWhere(Expression exp);
};

#endif
