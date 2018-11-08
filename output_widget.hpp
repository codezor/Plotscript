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

class GraphicsView;
class GraphicsScene;


class OutputWidget : public QWidget
{
	Q_OBJECT

public: OutputWidget(QWidget* parent = nullptr);
	//OutputWidget
	
private:
	// Graphics
	//QString Expression;
	QGraphicsScene *scene;
	QGraphicsView *view;
	//QGraphicsTextItem *tex;

	//QGraphicsEllipseItem *ellipse;

	
private slots:
	// Display Item
	void DisplayItem(QString Expression);
	void DisplayText(QString write, double x, double y, double rotation, double scale);
	void DisplayLine(double x1, double y1, double x2, double y2, double thickness);
	void DisplayPoint(double x, double y, double size);
	void DisplayClear();
	// Display none or nothing

signals:
	void ExpressionEvaluated(QString Expression);	
};

#endif
