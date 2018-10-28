#include "output_widget.hpp"
#include <QLayout>
#include <QWidget>
#include <QGraphicsTextItem>

OutputWidget::OutputWidget(QWidget* parent) : QWidget(parent){
	// Create a new scene 	
	scene = new QGraphicsScene;

	view = new QGraphicsView(scene, parent );
	
	auto layout = new QVBoxLayout;
	layout->addWidget(view);
	view->centerOn(0, 0);
	
	setLayout(layout);	
}

void OutputWidget::DisplayItem(QString Expression) {
	
	//scene->clear();
	
	scene->addText(Expression)->setPos(0,0);	
}

void OutputWidget::DisplayPoint(double x, double y, double size) {
		
	QPen pen;
	pen.setColor(Qt::black);	
	
	scene->addEllipse((x-size/2.0), (y-size/2.0), size, size, pen, QBrush(Qt::SolidPattern));	
}

void OutputWidget::DisplayLine(double x1, double y1, double x2, double y2, double thickness) {
	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(thickness);
	
	scene->addLine(x1, y1, x2, y2);
}

void OutputWidget::DisplayText(QString write, double x, double y) {
	
	write.remove("\"");
	scene->addText(write)->setPos(x, y);
}

void OutputWidget::DisplayClear() {
	scene->clear();
}