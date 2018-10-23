#include "output_widget.hpp"
#include <QLayout>
#include <QWidget>

OutputWidget::OutputWidget(QWidget* parent) : QWidget(parent){
	// Create a new scene 	
	scene = new QGraphicsScene;

	view = new QGraphicsView(scene, parent );
	scene->addText("DEZ NUTS");
	//scene->addEllipse(10,10,100,100, Qtblackpen)
	
	auto layout = new QVBoxLayout;
	layout->addWidget(view);
	setLayout(layout);
	
}
void OutputWidget::DisplayItem(std::string Expression) {
}