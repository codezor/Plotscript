#include "output_widget.hpp"
#include <QLayout>
#include <QWidget>
#include <QGraphicsTextItem>
OutputWidget::OutputWidget(QWidget* parent) : QWidget(parent){
	// Create a new scene 	
	scene = new QGraphicsScene;

	view = new QGraphicsView(scene, parent );
	//scene->addText("This is a box");
	//scene->addEllipse(10,10,100,100, Qtblackpen)
	
	auto layout = new QVBoxLayout;
	layout->addWidget(view);
	setLayout(layout);
	
}
void OutputWidget::DisplayItem(QString Expression) {
	//scene->removeItem();

	//addText =new QGraphicsTextItem(Expression);
	scene->addText(Expression);
	//scene->addItem(Expression);
}