/*! \ file input_widget.hpp
 */
#ifndef INPUT_WIDGET_HPP
#define INPUT_WIDGET_HPP
#include <QPlainTextEdit>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>

#include <string>

class InputWidget : public QPlainTextEdit
{
	Q_OBJECT

public: 
	InputWidget(QWidget* parent = nullptr);
	//std::string getUserInput();
	
private slots:
	void getInputText(std::string inputText);

signals: 
	void inputReady(std::string inputText);
	
protected:
	void keyPressEvent(QKeyEvent *event);
};

#endif
