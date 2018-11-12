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
	scene->addEllipse((x-size/2.0), (y-size/2.0), size, size, pen, QBrush(Qt::SolidPattern));	
	
}

void OutputWidget::DisplayLine(double x1, double y1, double x2, double y2, double thickness) {

	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(thickness);	
	scene->addLine(x1, y1, x2, y2, pen);
}

void OutputWidget::DisplayText(QString write, double x, double y, double rotation, double scale) {

	CrossHair();
	QPointF org(x, y);
	
	write.remove("\"");
	QFont font = QFont("Monospace");
	font.setStyleHint(QFont::TypeWriter);
	font.setPointSize(1);	
	
	QGraphicsTextItem* Text = scene->addText(write);
	Text->setFont(font);
	Text->setParent(scene);	

	
	Text->setScale(scale);	
		
	Text->moveBy(-Text->boundingRect().width() / 2, -Text->boundingRect().height() / 2);
	Text->setTransformOriginPoint(Text->boundingRect().width() / 2, Text->boundingRect().height() / 2);
	
	Text->setRotation(rotation * 180 / ( std::atan2(0, -1) ));
	Text->moveBy(x, y);
	qDebug() << "Text: Bounding Rect: " << Text->boundingRect() << "Position: " << Text->pos();
}

// Discrete Plot generation 
/*void OutputWidget::DisplayDiscretePlot()
{	   
	double N = 20;
	double A = 3;
	double B = 3;
	//double C = 2;
	//double D = 2;
	//double P = 0.5;  // pont size
	
	double scaleX = N / 2;	
	double xmin = scaleX * -1;
	double xmax = scaleX;
	double scaleY = N / 2;
	double ymin = scaleY *-1;;
	double ymax = scaleY;

	double xmiddle = ( xmax + xmin ) / 2;
	double ymiddle =(ymax + ymin) / 2;
	

	QFont f("Courier");
	f.setPointSize(1);
	
	QPen dataPen;
	dataPen.setColor(Qt::black);
	dataPen.setCosmetic(true);
	// Outer most lebel box
	//QRect labelBox;
	//labelBox.setRect(xmin - B, ymax + A, xmax + 2 * B, ymax + 2 * A);
	//QGraphicsScene *Labeles = new QGraphicsScene;
	//QGraphicsView *LabelArea = new QGraphicsView;

	//Labeles->setSceneRect(labelBox);
	//scene->addWidget(LabelArea);
	
	// middle bounding tick values should have floating point precesion
	//QRectF tickBox;	
	//tickBox.setRect(xmin - C, ymax + D, xmax + 2 * C, ymax + 2 * D);
	//QGraphicsScene *Style = new QGraphicsScene;
	//QGraphicsView *StyleArea = new QGraphicsView;
	//StyleArea->setScene(Style);
	//Style->setSceneRect(tickBox);
	//Labeles->addWidget(StyleArea);
	// Data bounding box
	QRect dataBoundBox;
	dataBoundBox.setRect(xmin, ymax, scaleX, scaleY);
	//QGraphicsScene *Plot = new QGraphicsScene;
	//QGraphicsView *PlotArea = new QGraphicsView;
	//PlotArea->setScene(Plot);
	//Plot->setSceneRect(dataBoundBox);
	//Style->addWidget(PlotArea);
	

	//scene->addRect(labelBox, dataPen);
	//scene->addRect(tickBox, dataPen);
	scene->addRect(dataBoundBox, dataPen);	

	
	QGraphicsTextItem *Tittle = scene->addText("Tittle");
	QGraphicsTextItem* XLabel = scene->addText("X Label");
	QGraphicsTextItem* YLabel = scene->addText("Y Label");
	
	Tittle->setPos(xmiddle, (ymax + 2 * A));
	Tittle->setScale(1);
	Tittle->setFont(f);

	XLabel->setPos(xmiddle, (ymin-2*A));
	XLabel->setScale(1);
	XLabel->setFont(f);
	
	YLabel->setPos((xmin-2*B), ymiddle);
	YLabel->setRotation(-90);
	YLabel->setScale(1);
	YLabel->setFont(f);
	//YLabel
	
	QLineF Xaxis;	
	Xaxis.setLine(xmin, ymiddle, xmax, ymiddle);	
	//Plot->addLine(Xaxis, dataPen);
	
	QLineF Yaxis;
	Yaxis.setLine(xmiddle, ymin, xmiddle, ymax);	
	//Plot->addLine(Yaxis, dataPen);
}
*/
void OutputWidget::DisplayClear() {
	scene->clear();
}

//void OutputWidget::resizeEvent(QResizeEvent *event) {

	//this->resizeEvent(event);
	//this->view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
//}
void OutputWidget::CrossHair()
{
	QPen P;
	P.setColor(Qt::black);
	P.setCosmetic(true);
	scene->addLine(QLineF(QPointF(-1, 0), QPointF(1, 0)), P)->setPos(0,0);
	scene->addLine(QLineF(QPointF(0, -1), QPointF(0, 1)), P)->setPos(0, 0);
	//qDebug() << "Croshair: Bounding Rect: " << Text->boundingRect() << "Position: " << Text->pos();

}