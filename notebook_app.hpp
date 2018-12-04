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
#include <QPushButton>
#include <QButtonGroup>
#include <QGraphicsGridLayout>
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
	void plotScriptInputReady(QString  InputText);
	void stopButtonPressed();
	void startButtonPressed();
	void resetButtonPressed();
	void interruptButtonPressed();
	void EnableInputWidget();

signals: 
	void ExpressionReady(QString Expression);
	void EllipseReady(double x, double  y, double size);
	void LineReady(double x1, double y1, double x2, double y2, double thickness);
	void TextReady(QString write, double x, double y, double rotation, double scale);
	
	void discretePlotReady(QString title, QString xlable, QString ylabel, double xmin, double xmax, double ymin, double ymax, double textscale);
	void ClearScene();
	void OutputWidgetDisplayed();
	//void StartButton();
	//void StopButton();

private:
	Interpreter interp;

	InputWidget* input;
	OutputWidget* output;
	QGridLayout * layout;
	QPushButton *startButton;
	QPushButton *stopButton;
	QPushButton *resetButton;
	QPushButton *interruptButton;

	std::thread * m_plotscript_thread_ptr { nullptr }; // = new std::thread(&Interpreter::parseStreamQueue, &interp);

	//bool m_interrupt{ false };

	void repl(std::string line);
	//void plotscript_thread_main(message_queue<std::string> &queue);
	void error(const std::string& err_str);
	void info(const std::string& err_str);

	void startUp(Interpreter& interp);
	void eval_from_stream(std::istream& stream);
	void eval_from_file(std::string filename);
	void eval_from_command(std::string argexp);
	
	// Determine what the expression type is and sends it to the approprate output 
	void whatGoesWhere(Expression exp);
	void outputPolling();
	void makeExpression(Expression exp);
	void makeLine(Expression exp);
	void makePoint(Expression exp);
	void makeText(Expression exp);
	void makeDiscretePlot(Expression exp);

};

#endif
