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
	QGraphicsTextItem *tex;
	
private slots:
	// Display Item
	void DisplayItem(QString Expression);
	// Display Text 
	// Display Line
	// Display ellipse
	// Display none or nothing

signals:
	void ExpressionEvaluated(QString Expression);

	
};

#endif
