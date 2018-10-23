/*! \ file output_widget.hpp
 */
#ifndef OUTPUT_WIDGET_HPP
#define OUTPUT_WIDGET_HPP
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLayout>
#include <QLayout>


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
	QGraphicsScene *scene;
	QGraphicsView *view;
	
private slots:
	void DisplayItem(std::string Expression);


signals:
	void ExpressionEvluated(std::string Expression);

	
};

#endif
