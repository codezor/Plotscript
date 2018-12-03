#include "input_widget.hpp"



InputWidget::InputWidget(QWidget* parent) : QPlainTextEdit(parent) {		
	
}

void InputWidget::keyPressEvent(QKeyEvent *event) 
{
	if((event->modifiers() == Qt::ShiftModifier)&& (event->key() == Qt::Key_Enter|| event->key() == Qt::Key_Return)) {
		m_inputText = QPlainTextEdit::toPlainText();
		emit inputReady(m_inputText);
		
		event->accept(); 
	}
	//else if(event->matches(QKeySequence::Copy))
	//{

	//}
	// All other keys should not take action
	else{			
		// Allow these key press events to type in the plaintextedit box
		QPlainTextEdit::keyPressEvent(event);
	}
}

