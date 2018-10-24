/*! \ file input_widget.hpp
 */
#ifndef INPUT_WIDGET_HPP
#define INPUT_WIDGET_HPP
#include <QPlainTextEdit>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>
#include <QString>
#include <string>

class InputWidget : public QPlainTextEdit
{
	Q_OBJECT

public: 
	InputWidget(QWidget* parent = nullptr);

private:
	QString m_inputText;
	
private slots:
	//void getInputText(QString inputText);

signals: 
	// When the shift+enter keys are pushed the text is put into the stream
	void inputReady(QString inputText);
	
protected:
	// this should probably be the signal
	void keyPressEvent(QKeyEvent *event);
};

#endif
