#include "input_widget.hpp"



InputWidget::InputWidget(QWidget* parent) : QPlainTextEdit(parent) {

	
	// toPlainText()
}

void InputWidget::keyPressEvent(QKeyEvent *event) //: keyPressEvent(QKeyEvent *e)
{
	bool is_enter_pressed = false;
	bool is_shift_pressed = false;
	//qDebug() << event->key() ;
	
	if ((event->modifiers() == Qt::ShiftModifier)&& (event->key() == Qt::Key_Enter|| event->key() == Qt::Key_Return)) {
		is_shift_pressed = true;
		is_enter_pressed = true;
		qDebug() << "shift + enter was pressed ";
		//void QWidget::focusInEvent(QFocusEvent *event)
		event->accept();
	}
	else
	{
		bool is_enter_pressed = false;
		bool is_shift_pressed = false;

		
		QPlainTextEdit::keyPressEvent(event);
	}

	
	if (is_enter_pressed && is_shift_pressed) {
		//do the thing
		qDebug() << "shift + enter";
		QPlainTextEdit::toPlainText();

	}
}
void InputWidget::getInputText(std::string inputText) {
}
