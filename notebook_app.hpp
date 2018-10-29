/*! \ file notebook_app.hpp
 */
#ifndef NOTEBOOK_APP_HPP
#define NOTEBOOK_APP_HPP
// See which of these I don't need
#include <QWidget>
#include <QLayout> // 
#include <QDebug> // Debug
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
	// Keypress in the input widget has occurred
	void plotScriptInputReady(QString  input);

signals: 
	void ExpressionReady(QString Expression);
	void EllipseReady(double x, double  y, double size);
	void LineReady(double x1, double y1, double x2, double y2, double thickness);
	void TextReady(QString write, double x, double y);
	void ClearScene();

private:
	InputWidget* input;
	OutputWidget* output;
	Interpreter interp;

	void repl(std::string line);
	void error(const std::string& err_str);
	void info(const std::string& err_str);

	void startUp(Interpreter& interp);
	void eval_from_stream(std::istream& stream);
	void eval_from_file(std::string filename);
	void eval_from_command(std::string argexp);
	
	// Determine what the expression type is and sends it to the approprate output 
	void whatGoesWhere(Expression exp);
	
	void makeExpression(Expression exp);
	void makeLine(Expression exp);
	void makePoint(Expression exp);
	void makeText(Expression exp);

};

#endif
