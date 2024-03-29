#include <QGraphicsLayout>
#include <QGraphicsEllipseItem>
#include <QTest>
#include "catch.hpp"
#include "notebook_app.hpp"
#include "input_widget.hpp"
#include "output_widget.hpp"
class NotebookTest : public QObject
{
	Q_OBJECT

private slots:
	// TODO: implement additional tests here
	void initTestCase();
	// TODO: implement additional tests here
	void testFindInputWidget();
	void testFindOutputWidget();
	void testInput();
	void testOutput();
	void testPoint();
	void testLine();
	void testString();
	void testDiscretePlotLayout();

private:
	NotebookApp noteBook; // Using for the initial test
	NotebookApp notebook;
	
};

void NotebookTest::initTestCase()
{
	//noteBook.show();
}
void NotebookTest::testFindInputWidget()
{

	auto input = noteBook.findChild<QPlainTextEdit*>("input");

	//QVERIFY2(input, "Could not find widget with name: 'input' ");	

}

void NotebookTest::testFindOutputWidget()
{

	auto output = noteBook.findChild<QWidget*>("output");

	//QVERIFY2(output, "Could not find widget with name: 'output' ");	

	auto outgraph = output->findChild<QGraphicsView*>();
	//QVERIFY2(outgraph, "Could not find QGraphicsView in the outputWidget. ");


	auto outscene = outgraph->findChild<QGraphicsScene*>();
	//QVERIFY2(outscene, "Could not find QGraphicsScene in the outputWidget. ");

}

void NotebookTest::testInput()
{

	auto input = noteBook.findChild<QPlainTextEdit*>("input");

	//QApplication::setActiveWindow(&noteBook);
	QTest::qWait(500);
	QTest::keyClicks(input, "(\"foo\")");
	QTest::qWait(500);

	QTest::keyClick(input, Qt::Key_Return, Qt::ShiftModifier);
	QTest::qWait(500);

	//QCOMPARE(input->toPlainText(), QString("(\"foo\")"));

	auto output = noteBook.findChild<QWidget*>("output");
	auto outgraph = output->findChild<QGraphicsView*>();
	auto outscene = outgraph->findChild<QGraphicsScene*>();
	auto textout = outscene->findChildren<QGraphicsTextItem*>();

	//QCOMPARE(textout, QString("(\"foo\")"));
	QTest::qWait(5000);
}

void NotebookTest::testOutput()
{
	auto output = noteBook.findChild<QWidget*>("output");
	output->isWidgetType();
	//output
}
void NotebookTest::testPoint()
{

	auto input = noteBook.findChild<QPlainTextEdit*>("input");
	input->clear();
	QTest::qWait(500);
	//QApplication::setActiveWindow(&noteBook);
	QTest::qWait(500);
	QTest::keyClicks(input, "(set-property \"size\" 20 (make-point 0 0))");
	QTest::qWait(500);

	QTest::keyClick(input, Qt::Key_Return, Qt::ShiftModifier);
	QTest::qWait(500);

	//QCOMPARE(input->toPlainText(), QString("(set-property \"size\" 20 (make-point 0 0))"));


	//auto output = noteBook.findChild<QWidget*>("output");
	//auto outgraph = output->findChild<QGraphicsView*>();
	//auto outscene = outgraph->findChild<QGraphicsScene*>();
	//auto elipse = outscene->findChild<QGraphicsEllipseItem>();
	//QTest::qWait(5000);
}

void NotebookTest::testLine()
{
	auto input = noteBook.findChild<QPlainTextEdit*>("input");
	input->clear();
	QTest::qWait(500);
	//QApplication::setActiveWindow(&noteBook);
	QTest::qWait(500);
	QTest::keyClicks(input, "(set-property \"thickness\" (4) (make-line (make-point 0 0)(make-point 20 20)))");
	QTest::qWait(500);

	QTest::keyClick(input, Qt::Key_Return, Qt::ShiftModifier);
	QTest::qWait(500);

	//QCOMPARE(input->toPlainText(), QString("(set-property \"thickness\" (4) (make-line (make-point 0 0)(make-point 20 20)))"));


	//auto output = noteBook.findChild<QWidget*>("output");
	//auto outgraph = output->findChild<QGraphicsView*>();
	//auto outscene = outgraph->findChild<QGraphicsScene*>();
	//auto elipse = outscene->findChild<QGraphicsLineItem>();
	//QTest::qWait(5000);

}

void NotebookTest::testString()
{

	auto input = noteBook.findChild<QPlainTextEdit*>("input");
	input->clear();
	QTest::qWait(500);
	//QApplication::setActiveWindow(&noteBook);
	QTest::qWait(500);
	QTest::keyClicks(input, "(set-property \"position\" (make-point 10 10) (make-text \"Hi\"))");
	QTest::qWait(500);

	QTest::keyClick(input, Qt::Key_Return, Qt::ShiftModifier);
	QTest::qWait(500);

	//QCOMPARE(input->toPlainText(), QString("(set-property \"position\" (make-point 10 10) (make-text \"Hi\"))"));


	//auto output = noteBook.findChild<QWidget*>("output");
	//auto outgraph = output->findChild<QGraphicsView*>();
	//auto outscene = outgraph->findChild<QGraphicsScene*>();
	//auto elipse = outscene->findChild<QGraphicsLineItem>();
	//QTest::qWait(5000);
}
/*
findLines - find lines in a scene contained within a bounding box
with a small margin
*/
int findLines(QGraphicsScene * scene, QRectF bbox, qreal margin)
{

	QPainterPath selectPath;

	QMarginsF margins(margin, margin, margin, margin);
	selectPath.addRect(bbox.marginsAdded(margins));
	scene->setSelectionArea(selectPath, Qt::ContainsItemShape);

	int numlines(0);
	foreach(auto item, scene->selectedItems())
	{
		if(item->type() == QGraphicsLineItem::Type)
		{
			numlines += 1;
		}
	}

	return numlines;
}

/*
findPoints - find points in a scene contained within a specified rectangle
*/
int findPoints(QGraphicsScene * scene, QPointF center, qreal radius)
{

	QPainterPath selectPath;
	selectPath.addRect(QRectF(center.x() - radius, center.y() - radius, 2 * radius, 2 * radius));
	scene->setSelectionArea(selectPath, Qt::ContainsItemShape);

	int numpoints(0);
	foreach(auto item, scene->selectedItems())
	{
		if(item->type() == QGraphicsEllipseItem::Type)
		{
			numpoints += 1;
		}
	}

	return numpoints;
}

/*
findText - find text in a scene centered at a specified point with a given
rotation and string contents
*/
int findText(QGraphicsScene * scene, QPointF center, qreal rotation, QString contents)
{

	int numtext(0);
	foreach(auto item, scene->items(center))
	{
		if(item->type() == QGraphicsTextItem::Type)
		{
			QGraphicsTextItem * text = static_cast< QGraphicsTextItem * >( item );
			if(( text->toPlainText() == contents ) &&
				( text->rotation() == rotation ) &&
				( text->pos() + text->boundingRect().center() == center ))
			{
				numtext += 1;
			}
		}
	}

	return numtext;
}

/*
intersectsLine - find lines in a scene that intersect a specified rectangle
*/
int intersectsLine(QGraphicsScene * scene, QPointF center, qreal radius)
{

	QPainterPath selectPath;
	selectPath.addRect(QRectF(center.x() - radius, center.y() - radius, 2 * radius, 2 * radius));
	scene->setSelectionArea(selectPath, Qt::IntersectsItemShape);

	int numlines(0);
	foreach(auto item, scene->selectedItems())
	{
		if(item->type() == QGraphicsLineItem::Type)
		{
			numlines += 1;
		}
	}

	return numlines;
}


void NotebookTest::testDiscretePlotLayout()
{

	std::string program = R"( 
(discrete-plot (list (list -1 -1) (list 1 1)) 
    (list (list "title" "The Title") 
          (list "abscissa-label" "X Label") 
          (list "ordinate-label" "Y Label") ))
)";

	auto inputWidget = notebook.findChild<InputWidget *>("input");
	auto outputWidget = notebook.findChild<OutputWidget *>("output");
	inputWidget->setPlainText(QString::fromStdString(program));
	QTest::keyClick(inputWidget, Qt::Key_Return, Qt::ShiftModifier);

	auto view = outputWidget->findChild<QGraphicsView *>();
	QVERIFY2(view, "Could not find QGraphicsView as child of OutputWidget");

	auto scene = view->scene();

	// first check total number of items
	// 8 lines + 2 points + 7 text = 17
	auto items = scene->items();
	QCOMPARE(items.size(), 17);

	// make them all selectable
	foreach(auto item, items)
	{
		item->setFlag(QGraphicsItem::ItemIsSelectable);
	}

	double scalex = 20.0 / 2.0;
	double scaley = 20.0 / 2.0;

	double xmin = scalex * -1;
	double xmax = scalex * 1;
	double ymin = scaley * -1;
	double ymax = scaley * 1;
	double xmiddle = ( xmax + xmin ) / 2;
	double ymiddle = ( ymax + ymin ) / 2;

	// check title
	QCOMPARE(findText(scene, QPointF(xmiddle, -( ymax + 3 )), 0, QString("The Title")), 1);

	// check abscissa label
	QCOMPARE(findText(scene, QPointF(xmiddle, -( ymin - 3 )), 0, QString("X Label")), 1);

	// check ordinate label
	QCOMPARE(findText(scene, QPointF(xmin - 3, -ymiddle), -90, QString("Y Label")), 1);

	// check abscissa min label
	QCOMPARE(findText(scene, QPointF(xmin, -( ymin - 2 )), 0, QString("-1")), 1);

	// check abscissa max label
	QCOMPARE(findText(scene, QPointF(xmax, -( ymin - 2 )), 0, QString("1")), 1);

	// check ordinate min label
	QCOMPARE(findText(scene, QPointF(xmin - 2, -ymin), 0, QString("-1")), 1);

	// check ordinate max label
	QCOMPARE(findText(scene, QPointF(xmin - 2, -ymax), 0, QString("1")), 1);

	// check the bounding box bottom
	QCOMPARE(findLines(scene, QRectF(xmin, -ymin, 20, 0), 0.1), 1);

	// check the bounding box top
	QCOMPARE(findLines(scene, QRectF(xmin, -ymax, 20, 0), 0.1), 1);

	// check the bounding box left and (-1, -1) stem
	QCOMPARE(findLines(scene, QRectF(xmin, -ymax, 0, 20), 0.1), 2);

	// check the bounding box right and (1, 1) stem
	QCOMPARE(findLines(scene, QRectF(xmax, -ymax, 0, 20), 0.1), 2);

	// check the abscissa axis
	QCOMPARE(findLines(scene, QRectF(xmin, 0, 20, 0), 0.1), 1);

	// check the ordinate axis 
	QCOMPARE(findLines(scene, QRectF(0, -ymax, 0, 20), 0.1), 1);

	// check the point at (-1,-1)
	QCOMPARE(findPoints(scene, QPointF(-10, 10), 0.6), 1);

	// check the point at (1,1)
	QCOMPARE(findPoints(scene, QPointF(10, -10), 0.6), 1);
}

QTEST_MAIN(NotebookTest)
#include "notebook_test.moc"