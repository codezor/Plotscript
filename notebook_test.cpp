#include <QTest>
#include <QWidget>
#include <QPlainTextEdit>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLayout>
#include <QGraphicsEllipseItem>
#include "notebook_app.hpp"
class NotebookTest : public QObject
{
  Q_OBJECT

private slots:

  void initTestCase();

  // TODO: implement additional tests here
  void testFindInputWidget();
  void testFindOutputWidget();
  void testInput();
  void testOutput();
  void testPoint();
  void testLine();
  void testString();


private: 
	//ExampleWidget widget;
	NotebookApp noteBook;
	//auto input = noteBook.findChild<QPlainTextEdit*>("input");
};

void NotebookTest::initTestCase()
{
	noteBook.show();
	//QVERIFY(true);
}
void NotebookTest::testFindInputWidget() {
	
	auto input = noteBook.findChild<QPlainTextEdit*>("input");

	QVERIFY2(input, "Could not find widget with name: 'input' ");	
	
}

void NotebookTest::testFindOutputWidget() {
		
	auto output = noteBook.findChild<QWidget*>("output");

	QVERIFY2(output, "Could not find widget with name: 'output' ");	
	
	auto outgraph = output->findChild<QGraphicsView*>();
	QVERIFY2(outgraph, "Could not find QGraphicsView in the outputWidget. ");
	

	auto outscene = outgraph->findChild<QGraphicsScene*>();
	QVERIFY2(outscene, "Could not find QGraphicsScene in the outputWidget. ");

}

void NotebookTest::testInput() {	
	
	auto input = noteBook.findChild<QPlainTextEdit*>("input");
	
	//QApplication::setActiveWindow(&noteBook);
	QTest::qWait(500);
	QTest::keyClicks(input, "(\"foo\")");	
	QTest::qWait(500);
	
	QTest::keyClick(input, Qt::Key_Return, Qt::ShiftModifier);
	QTest::qWait(500);
	
	QCOMPARE(input->toPlainText(), QString("(\"foo\")"));
	
	auto output = noteBook.findChild<QWidget*>("output");
	auto outgraph = output->findChild<QGraphicsView*>();	   
	auto outscene = outgraph->findChild<QGraphicsScene*>();
	auto textout = outscene->findChildren<QGraphicsTextItem*>();
	
	//QCOMPARE(textout, QString("(\"foo\")"));
	QTest::qWait(5000);
}

void NotebookTest::testOutput() {
	auto output = noteBook.findChild<QWidget*>("output");
	output->isWidgetType();
	//output
}
void NotebookTest::testPoint() {

	auto input = noteBook.findChild<QPlainTextEdit*>("input");
	input->clear();
	QTest::qWait(500);
	//QApplication::setActiveWindow(&noteBook);
	QTest::qWait(500);
	QTest::keyClicks(input, "(set-property \"size\" 20 (make-point 0 0))");
	QTest::qWait(500);

	QTest::keyClick(input, Qt::Key_Return, Qt::ShiftModifier);
	QTest::qWait(500);

	QCOMPARE(input->toPlainText(), QString("(set-property \"size\" 20 (make-point 0 0))"));


	//auto output = noteBook.findChild<QWidget*>("output");
	//auto outgraph = output->findChild<QGraphicsView*>();
	//auto outscene = outgraph->findChild<QGraphicsScene*>();
	//auto elipse = outscene->findChild<QGraphicsEllipseItem>();
	//QTest::qWait(5000);
}

void NotebookTest::testLine() {
	auto input = noteBook.findChild<QPlainTextEdit*>("input");
	input->clear();
	QTest::qWait(500);
	//QApplication::setActiveWindow(&noteBook);
	QTest::qWait(500);
	QTest::keyClicks(input, "(set-property \"thickness\" (4) (make-line (make-point 0 0)(make-point 20 20)))");
	QTest::qWait(500);

	QTest::keyClick(input, Qt::Key_Return, Qt::ShiftModifier);
	QTest::qWait(500);

	QCOMPARE(input->toPlainText(), QString("(set-property \"thickness\" (4) (make-line (make-point 0 0)(make-point 20 20)))"));


	//auto output = noteBook.findChild<QWidget*>("output");
	//auto outgraph = output->findChild<QGraphicsView*>();
	//auto outscene = outgraph->findChild<QGraphicsScene*>();
	//auto elipse = outscene->findChild<QGraphicsLineItem>();
	//QTest::qWait(5000);

}

void NotebookTest::testString() {

	auto input = noteBook.findChild<QPlainTextEdit*>("input");
	input->clear();
	QTest::qWait(500);
	//QApplication::setActiveWindow(&noteBook);
	QTest::qWait(500);
	QTest::keyClicks(input, "(set-property \"position\" (make-point 10 10) (make-text \"Hi\"))");
	QTest::qWait(500);

	QTest::keyClick(input, Qt::Key_Return, Qt::ShiftModifier);
	QTest::qWait(500);

	QCOMPARE(input->toPlainText(), QString("(set-property \"position\" (make-point 10 10) (make-text \"Hi\"))"));


	//auto output = noteBook.findChild<QWidget*>("output");
	//auto outgraph = output->findChild<QGraphicsView*>();
	//auto outscene = outgraph->findChild<QGraphicsScene*>();
	//auto elipse = outscene->findChild<QGraphicsLineItem>();
	//QTest::qWait(5000);
}


QTEST_MAIN(NotebookTest)
#include "notebook_test.moc"
