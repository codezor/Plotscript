#include "input_widget.hpp"



InputWidget::InputWidget(QWidget* parent) : QPlainTextEdit(parent) {

	
	// toPlainText()
}

void InputWidget::keyPressEvent(QKeyEvent *event) //: keyPressEvent(QKeyEvent *e)
{
	//bool is_enter_pressed = false;
	//bool is_shift_pressed = false;
	//qDebug() << event->key() ;
	
	if ((event->modifiers() == Qt::ShiftModifier)&& (event->key() == Qt::Key_Enter|| event->key() == Qt::Key_Return)) {
		//is_shift_pressed = true;
		//is_enter_pressed = true;

		qDebug() << "shift + enter";
		m_inputText = QPlainTextEdit::toPlainText();
		emit inputReady(m_inputText);
		// Debugging stament 
		//qDebug() << "shift + enter was pressed ";		
		event->accept(); // why did I use this
	}
	// All pther keys should not take action
	else{
		
		//bool is_enter_pressed = false;
		//bool is_shift_pressed = false;	
		// Allow these key press events to type in the plaintextedit box
		QPlainTextEdit::keyPressEvent(event);
	}

	// Do action when the shift+enter is done 
	//if (is_enter_pressed && is_shift_pressed) {
		//do the thing
		//qDebug() << "shift + enter";
		//m_inputText = QPlainTextEdit::toPlainText();
		//emit inputReady(m_inputText);

	//}
}

// Input is ready
//void InputWidget::inputReady(QString inputText) {
	//inputText = inputText;
	
//}
