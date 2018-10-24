/*! \ file notebook_app.hpp
 */
#ifndef NOTEBOOK_APP_HPP
#define NOTEBOOK_APP_HPP

#include <QWidget>
#include <QLayout>
#include <QDebug>


#include"input_widget.hpp"
#include"output_widget.hpp"
class NotebookApp : public QWidget
{
	Q_OBJECT

public:
	NotebookApp(QWidget* parent = nullptr);
	


private slots:
	// what are this 
	void plotScriptEvaluated(QString output);
	// Keypress in the input widget has occurred
	void plotScriptInputReady(QString  input);

signals: 
	void ExpressionReady(QString Expression);

private:
	InputWidget* inputwidget;
	OutputWidget* outputwidget;
	//setchild
};

#endif
