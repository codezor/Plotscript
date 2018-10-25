#include "input_widget.hpp"



InputWidget::InputWidget(QWidget* parent) : QPlainTextEdit(parent) {

	
	// toPlainText()
}

void InputWidget::keyPressEvent(QKeyEvent *event) //: keyPressEvent(QKeyEvent *e)
{		
	if ((event->modifiers() == Qt::ShiftModifier)&& (event->key() == Qt::Key_Enter|| event->key() == Qt::Key_Return)) {
		
		qDebug() << "shift + enter";
		m_inputText = QPlainTextEdit::toPlainText();
		emit inputReady(m_inputText);
		// Debugging stament 
		//qDebug() << "shift + enter was pressed ";		
		event->accept(); // why did I use this
	}
	// All other keys should not take action
	else{
			
		// Allow these key press events to type in the plaintextedit box
		QPlainTextEdit::keyPressEvent(event);
	}
}

// Input is ready
//void InputWidget::inputReady(QString inputText) {
	//inputText = inputText;
	
//}
