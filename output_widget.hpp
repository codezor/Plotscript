/*! \ file output_widget.hpp
 */
#ifndef OUTPUT_WIDGET_HPP
#define OUTPUT_WIDGET_HPP
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLayout>

#include <QLayout>
#include <QString>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <string>
#include <QDebug> // Debug

class GraphicsView;
class GraphicsScene;


class OutputWidget : public QWidget
{
	Q_OBJECT

public: OutputWidget(QWidget* parent = nullptr);
	//OutputWidget
	
private:
	// Graphics
	QGraphicsScene *scene;
	QGraphicsView *view;
	

	// maybe a text formatting function
	void CrossHair();
	
	//void resizeEvent(QResizeEvent *event);
	
private slots:
	// Display Item
	void DisplayItem(QString Expression);
	void DisplayText(QString write, double x, double y, double rotation, double scale);
	void DisplayLine(double x1, double y1, double x2, double y2, double thickness);
	void DisplayPoint(double x, double y, double size);
	//void DisplayDiscretePlot();
	void DisplayClear();
	// Display none or nothing

signals:
	void ExpressionEvaluated(QString Expression);	
};

#endif
