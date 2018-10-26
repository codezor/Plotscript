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
	//view->centerOn(scene);
	//view->centerOn(&scene);
	view->centerOn(0, 0);
	setLayout(layout);
	
}
void OutputWidget::DisplayItem(QString Expression) {
	//scene->removeItem();
	scene->clear();
	
	//QGraphicsTextItem tex(Expression);
	scene->addText(Expression)->setPos(0,0);
	
	//scene->addItem(Expression);
}