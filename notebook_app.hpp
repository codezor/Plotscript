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
	//setchild
	void repl(std::string line);
	void error(const std::string& err_str);
};

#endif
