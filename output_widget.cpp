#include "output_widget.hpp"
#include <QLayout>
#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFont>
#include <QCursor>
#include <QGraphicsTextItem>
#include <QPointF>
#include <cmath>

OutputWidget::OutputWidget(QWidget* parent) : QWidget(parent){
	
	// Create a new scene 	
	scene = new QGraphicsScene;
	view = new QGraphicsView(scene, parent );
	scene->setParent(view);
	// Locate 
	auto layout = new QVBoxLayout;
	layout->addWidget(view);
	view->centerOn(0, 0);
	
	setLayout(layout);	
}

void OutputWidget::DisplayItem(QString Expression) {

	QGraphicsTextItem* Text = scene->addText(Expression);// ->setPos(0, 0);
	Text->setParent(scene);
	Text->setPos(0, 0);
}

void OutputWidget::DisplayPoint(double x, double y, double size) {
	
	QPen pen;
	pen.setColor(Qt::black);
	//scene->addItem(&ellipse);
	//QBrush brush(Qt::SolidPattern);
	//pen.setBrush(QBrush(Qt::SolidPattern));

	scene->addEllipse((x-size/2.0), (y-size/2.0), size, size, pen, QBrush(Qt::SolidPattern));	
}

void OutputWidget::DisplayLine(double x1, double y1, double x2, double y2, double thickness) {
	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(thickness);
	
	scene->addLine(x1, y1, x2, y2, pen);
}

void OutputWidget::DisplayText(QString write, double x, double y, double rotation, double scale) {
	
	write.remove("\"");
	QFont f("Courier");
	f.setPointSize(1);
	QGraphicsTextItem* Text = scene->addText(write);
	Text->setFont(f);
	Text->setParent(scene);
	Text->setPos(x, y);
	
	Text->setRotation(rotation);
	//Text->setRotation(rotation* 180/ (std::atan2(0, -1)));
	Text->setScale(scale);
	
}

void OutputWidget::DisplayClear() {
	scene->clear();
}