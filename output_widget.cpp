#include "output_widget.hpp"
#include <QLayout>
#include <QWidget>
#include <QResizeEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFont>
#include <QCursor>
#include <QGraphicsTextItem>
#include <QPointF>
#include <cmath>

#include <QDebug> // Debug

OutputWidget::OutputWidget(QWidget* parent) : QWidget(parent){
	
	// Create a new scene 	
	scene = new QGraphicsScene;
	view = new QGraphicsView(scene, parent );
	scene->setParent(view);
	
	// Locate 
	auto layout = new QVBoxLayout();
	layout->addWidget(view);
	view->centerOn(0, 0);
	
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	
	setLayout(layout);	
}

void OutputWidget::DisplayItem(QString Expression) {
		
	//QFont f("Courier");
	//f.setPointSize(1);
	QGraphicsTextItem* Text = scene->addText(Expression);// ->setPos(0, 0);
	Text->setParent(scene);
	Text->setPos(0,0);
	
	//qDebug ()<< "Bounding Rect: "<<Text->boundingRect() << "Position: "<< Text->pos();
}

void OutputWidget::DisplayPoint(double x, double y, double size) {
	CrossHair();
	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(0);
	QGraphicsEllipseItem *Point =scene->addEllipse((x), (y), size, size, pen, QBrush(Qt::SolidPattern));
	Point->moveBy(-Point->boundingRect().width() / 2, -Point->boundingRect().height() / 2);
	Point->setTransformOriginPoint(Point->boundingRect().width() / 2, Point->boundingRect().height() / 2);
	//Title->moveBy(xmiddle, ( -scaleY * ymax - A ));
	qDebug() << "Pount: Position: " << Point->boundingRect();
}

void OutputWidget::DisplayLine(double x1, double y1, double x2, double y2, double thickness) {

	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(thickness);	
	QGraphicsLineItem *Line = scene->addLine(x1, y1, x2, y2, pen);
	//Line->moveBy(-Line->boundingRect().width() / 2, -Line->boundingRect().height() / 2);
	//Line->setTransformOriginPoint(Line->boundingRect().width() / 2, Line->boundingRect().height() / 2);
	
	qDebug() << "Line: Position: " << Line->boundingRect();
}

void OutputWidget::DisplayText(QString write, double x, double y, double rotation, double scale) {

	
	QPointF org(x, y);
	
	write.remove("\"");
	QFont font = QFont("Monospace");
	font.setStyleHint(QFont::TypeWriter);
	font.setPointSize(1);	
	
	QGraphicsTextItem* Text = scene->addText(write);
	Text->setFont(font);
	Text->setParent(scene);	

	
	Text->setScale(scale);	
		
	//Text->moveBy(-Text->boundingRect().width() / 2, -Text->boundingRect().height() / 2);
	//Text->setTransformOriginPoint(Text->boundingRect().width() / 2, Text->boundingRect().height() / 2);
	
	Text->setRotation(rotation * 180 / ( std::atan2(0, -1) ));
	Text->moveBy(x, y);
	//CrossHair();
	qDebug() << "scene: Position: " << scene->sceneRect();
	qDebug() << "Text: Bounding Rect: " << Text->boundingRect() << "Position: " << Text->pos();
}

// Discrete Plot generation 
void OutputWidget::DisplayDiscretePlot(QString title, QString xlable, QString ylabel, double xmin, double xmax, double ymin, double ymax, double textscale)
{
	double N = 20;
	double A = 3;
	double B = 3;
	double C = 2;
	double D = 2;
	//double P = 0.5;  // pont size

	
	

	double xmiddle = ( xmax + xmin ) / 2;
	double ymiddle = ( ymax + ymin ) / 2;
	double scaleX = 20 / ( xmax - xmin );
	double scaleY = 20 / ( ymax - ymin );

	QFont f = QFont("Monospace");
	f.setStyleHint(QFont::TypeWriter);
	f.setPointSize(1);



	QPen dataPen;
	dataPen.setColor(Qt::black);
	dataPen.setCosmetic(true);
	
	QRect dataBoundBox;
	dataBoundBox.setRect(xmin, ymin, N, N);
		
	title.remove("\"");
	xlable.remove("\"");
	ylabel.remove("\"");
	QGraphicsTextItem *Title = scene->addText(title);
	QGraphicsTextItem* XLabel = scene->addText(xlable);
	QGraphicsTextItem* YLabel = scene->addText(ylabel);

	Title->setScale(textscale );
	Title->setFont(f);

	Title->moveBy(-Title->boundingRect().width() / 2, -Title->boundingRect().height() / 2);
	Title->setTransformOriginPoint(Title->boundingRect().width() / 2, Title->boundingRect().height() / 2);
	Title->moveBy(xmiddle, ( -scaleY* ymax - A ));
	qDebug() << "Title: Position: " << Title->boundingRect();
	XLabel->setScale(textscale);
	XLabel->setFont(f);
	XLabel->moveBy(-XLabel->boundingRect().width() / 2, -XLabel->boundingRect().height() / 2);
	XLabel->setTransformOriginPoint(XLabel->boundingRect().width() / 2, XLabel->boundingRect().height() / 2);
	XLabel->moveBy(xmiddle, ( -scaleY * ymin + A ));
	qDebug() << "Xlabel: Position: " << XLabel->boundingRect();
	YLabel->setScale(textscale);
	YLabel->setFont(f);
	YLabel->moveBy(-YLabel->boundingRect().width() / 2, -YLabel->boundingRect().height() / 2);
	YLabel->setTransformOriginPoint(YLabel->boundingRect().width() / 2, YLabel->boundingRect().height() / 2);
	YLabel->setRotation(-90);
	YLabel->moveBy(( scaleX * xmin - B ), ymiddle);
	qDebug() << "Ylabel: Position: " << YLabel->boundingRect();
	CrossHair();
	qDebug() << "scene: Position: " << scene->sceneRect();


	// Adding tic marks
	QString XminT = QString::fromStdString(std::to_string(( int )xmin));
	QString XmaxT = QString::fromStdString(std::to_string(( int )xmax));
	QString YminT = QString::fromStdString(std::to_string(( int )ymin));
	QString YmaxT = QString::fromStdString(std::to_string(( int )ymax));
	
	QGraphicsTextItem *XMIN = scene->addText(XminT);
	QGraphicsTextItem *XMAX = scene->addText(XmaxT);
	QGraphicsTextItem *YMIN = scene->addText(YminT);
	QGraphicsTextItem *YMAX = scene->addText(YmaxT);
	
	XMIN->setScale(textscale);
	XMIN->setFont(f);

	XMIN->moveBy(-XMIN->boundingRect().width() / 2, -XMIN->boundingRect().height() / 2);
	XMIN->setTransformOriginPoint(XMIN->boundingRect().width() / 2, XMIN->boundingRect().height() / 2);
	XMIN->moveBy(scaleX* xmin, ( -scaleY * ymin  +C));
	qDebug() << "Xmin: Position: " << XMIN->pos();

	XMAX->setScale(textscale);
	XMAX->setFont(f);

	XMAX->moveBy(-XMAX->boundingRect().width() / 2, -XMAX->boundingRect().height() / 2);
	XMAX->setTransformOriginPoint(XMAX->boundingRect().width() / 2, XMAX->boundingRect().height() / 2);
	XMAX->moveBy(scaleX * xmax , ( -scaleY * ymin + C));
	qDebug() << "Xmax: Position: " << XMAX->pos();
	
	YMIN->setScale(textscale);
	YMIN->setFont(f);

	YMIN->moveBy(-YMIN->boundingRect().width() / 2, -YMIN->boundingRect().height() / 2);
	YMIN->setTransformOriginPoint(YMIN->boundingRect().width() / 2, YMIN->boundingRect().height() / 2);
	YMIN->moveBy(scaleX * xmin -D, ( -scaleY * ymin  ));
	qDebug() << "Ymin: Position: " << YMIN->pos();
	
	YMAX->setScale(textscale);
	YMAX->setFont(f);
	
	YMAX->setTransformOriginPoint(YMAX->boundingRect().width() / 2, YMAX->boundingRect().height() / 2);
	YMAX->moveBy(-YMAX->boundingRect().width() / 2, -YMAX->boundingRect().height() / 2);
	
	YMAX->moveBy(scaleX * xmin - D, ( -scaleY * ymax ));
	qDebug() << "Ymax: Position: " << YMAX->pos()<<YMAX->boundingRect();
}

void OutputWidget::DisplayClear() {
	scene->clear();
}
void OutputWidget::resizeEvent(QResizeEvent *event) {
	this->view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
	event->accept();
}
void OutputWidget::CrossHair()
{
	QPen P;
	P.setColor(Qt::black);
	P.setCosmetic(true);
	scene->addLine(QLineF(QPointF(-1, 0), QPointF(1, 0)), P)->setPos(0,0);
	scene->addLine(QLineF(QPointF(0, -1), QPointF(0, 1)), P)->setPos(0, 0);
	//qDebug() << "Croshair: Bounding Rect: " << Text->boundingRect() << "Position: " << Text->pos();

}