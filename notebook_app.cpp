
#include "notebook_app.hpp"

// should run the application similair to plocript
NotebookApp::NotebookApp(QWidget* parent)
	: QWidget(parent)
{
	
	input = new InputWidget;
	output = new OutputWidget;
	input->setObjectName("input");
	output->setObjectName("output");
	
	input->setParent(parent);
	output->setParent(parent);
	
	//Start Up file should be called before input is avaliable	

	startUp(interp);

	// The user clicks shift+enter
	QObject::connect(input,SIGNAL(inputReady(QString)),this, SLOT(plotScriptInputReady(QString)));

	// When the plot script is evaluate it will send back
	QObject::connect(this, SIGNAL(ExpressionReady(QString)), output, SLOT(DisplayItem(QString)));

	// Connecting elipse signal to Elipse output
	QObject::connect(this, SIGNAL(EllipseReady(double, double, double)), output, SLOT(DisplayPoint(double, double, double)));

	// Conect the line
	QObject::connect(this, SIGNAL(LineReady(double, double, double, double , double )), output, SLOT(DisplayLine(double, double, double, double, double)));

	// Draw the text
	QObject::connect(this, SIGNAL(TextReady(QString, double , double, double, double)), output, SLOT(DisplayText(QString, double, double, double, double)));

	// Draw the Discrete plot
	QObject::connect(this, SIGNAL(discretePlotReady()), output, SLOT(DisplayDiscretePlot()));


	// Clear the display
	QObject::connect(this, SIGNAL(ClearScene()), output, SLOT(DisplayClear()));

	auto layout = new QGridLayout();
	layout->addWidget(input, 0, 0);
	layout->addWidget(output, 1, 0);
	
	setLayout(layout);
	

}
void NotebookApp::error(const std::string& err_str) {
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	
	outstream << "Error: " << err_str;
	out = outstream.str();
	TextforOut = QString::fromStdString(out);

	///Now Emmit the error signal 
	emit ExpressionReady(TextforOut);
	//std::cerr << "Error: " << err_str << std::endl;
}

void NotebookApp::info(const std::string& err_str)
{
	
	std::stringstream outstream;
	std::string out;
	QString TextforOut;

	outstream << "Info: " << err_str;
	out = outstream.str();
	TextforOut = QString::fromStdString(out);

	// Now Emmit the error signal 
	emit ExpressionReady(TextforOut);
}


void NotebookApp::startUp(Interpreter& interp) {
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	
	std::ifstream ifs(STARTUP_FILE);

	if (!interp.parseStream(ifs)) {
		out = "Error: Invalid Expression. Could not parse.";
		// Send a Parse error to output
		TextforOut = QString::fromStdString(out);
		
	}
	else {
		try {
			Expression exp = interp.evaluate();
			outstream << exp;
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
		}
		catch (const SemanticError& ex) {
			outstream << ex.what() << std::endl;
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
			// return EXIT_FAILURE;
		}
	}
	emit ExpressionReady(TextforOut);
}

void NotebookApp::eval_from_stream(std::istream& stream) {
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	Interpreter interp;
	

	if (!interp.parseStream(stream)) {
		out = "Error: Invalid Expression. Could not parse.";
		// Send a Parse error to output
		TextforOut = QString::fromStdString(out);
		
	}
	else {
		try {
			Expression exp = interp.evaluate();
			outstream<< exp;
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
		}
		catch (const SemanticError& ex) {
			outstream<< "Error: " << ex.what();
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
			//std::cerr << ex.what() << std::endl;
		}
	}
	emit ExpressionReady(TextforOut);
}

void NotebookApp::eval_from_file(std::string filename) {
	std::ifstream ifs(filename);
	
	std::stringstream outstream;
	std::string out;
	QString TextforOut;

	if (!ifs) {

		out = "Error: Invalid Expression. Could not parse.";
		// Send a Parse error to output
		TextforOut = QString::fromStdString(out);
		emit ExpressionReady(TextforOut);
	}

	eval_from_stream(ifs);
}

void NotebookApp::eval_from_command(std::string argexp) {
	
	std::istringstream expression(argexp);
	eval_from_stream(expression);
}

//  REPL is a reapeated read-eval-print loop 
void NotebookApp::repl(std::string line) //TODO: rename since this technically isn't a loop now
{	
	std::stringstream outstream;
	std::string out;
	QString TextforOut;	
	std::istringstream expression(line);	
	
	if (!interp.parseStream(expression)) {
		out ="Error: Invalid Expression. Could not parse.";
		// Send a Parse error to output
		TextforOut = QString::fromStdString(out);
		emit ExpressionReady(TextforOut);
	}

	else {
		try {
			Expression exp = interp.evaluate();
			whatGoesWhere(exp);			
		}
		catch (const SemanticError& ex) {
			outstream  << ex.what();
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
			emit ExpressionReady(TextforOut);
		}		
	}	
}

void NotebookApp::plotScriptInputReady(QString input) {
	
	// Turn input to string	
	std::string line = input.toStdString();
	
	// Clear output screan
	emit(ClearScene());
	
	// evaluate input
	repl(line);
}

void NotebookApp::whatGoesWhere(Expression exp) {	
		
	if (!exp.isPropertyListEmpty()) {
		std::string propertyKey = "\"object-name\"";
		Expression objectName;
		objectName = exp.getPropertyList(propertyKey);
		if (objectName.isHeadString() )
		{
			// Draw point
			if (objectName.head().asString() == "\"point\""){
				makePoint(exp);
			}
			// Draw line
			if (objectName.head().asString() == "\"line\"") {
				makeLine(exp);
			}
			//DrawText
			if (objectName.head().asString() == "\"text\"") {
				makeText(exp);
			}
			// Draw Plot discrete-plot
			if (objectName.head().asString() == "\"discrete-plot\"") {
				//
				makeDiscretePlot();
			}
		}
	}
	else if (exp.isHeadList()) {
		for (auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e) {
			whatGoesWhere(*e);
		}
	}

	// Regular plot script things 
	else
	{
		makeExpression(exp);				
	}
}

void NotebookApp::makeExpression(Expression exp) {
	
	std::stringstream outstream;
	std::string out;
	QString TextforOut;
	if (!exp.isHeadLambda() || !exp.isHeadSymbol()) {
		outstream << exp;
		out = outstream.str();
		TextforOut = QString::fromStdString(out);
		emit ExpressionReady(TextforOut);
	}
}

void NotebookApp::makeLine(Expression exp){

	Expression lineThickness;
	lineThickness = exp.getPropertyList("\"thickness\"");
	if (lineThickness.head().asNumber()<0) {
		// emit the signal for the error message
	}
	auto point1 = exp.tailConstBegin();
	auto point2 = exp.tailConstEnd();
	std::vector<Expression> line(point1, point2);

	auto p1x = line[0].tailConstBegin();
	auto p1y = line[0].tailConstEnd();
	auto p2x = line[1].tailConstBegin();
	auto p2y = line[1].tailConstEnd();

	std::vector<Expression> p1(p1x, p1y);
	std::vector<Expression> p2(p2x, p2y);

	emit(LineReady(p1[0].head().asNumber(), p1[1].head().asNumber(), p2[0].head().asNumber(), 
		p2[1].head().asNumber(), lineThickness.head().asNumber()));

}

void NotebookApp::makePoint(Expression exp){
	
	Expression pointSize;
	//Expression 
	pointSize = exp.getPropertyList("\"size\"");
	if (pointSize.head().asNumber() < 0) {
		// emit the signal for the error message
	}
	auto xcor = exp.tailConstBegin();
	auto ycor = exp.tailConstEnd();
	std::vector<Expression> cordinates(xcor, ycor);

	Expression x = cordinates.front();
	Expression y = cordinates.back();

	emit(EllipseReady(x.head().asNumber(), y.head().asNumber(), pointSize.head().asNumber()));
}

void NotebookApp::makeText(Expression exp){
	
	Expression textPosition;
	textPosition = exp.getPropertyList("\"position\"");	

	Expression textScale;
	textScale = exp.getPropertyList("\"text-scale\"");	
	
	Expression textRotation;
	textRotation = exp.getPropertyList("\"text-rotation\"");	
	
	auto xcor = textPosition.tailConstBegin();
	auto ycor = textPosition.tailConstEnd();
	std::vector<Expression> cordinates(xcor, ycor);
	QString words = QString::fromStdString(exp.head().asString());

	textPosition = exp.getPropertyList("\"position\"");
	//textPosition = exp.getPropertyList("\"position\"");
	emit(TextReady(words, cordinates[0].head().asNumber(), cordinates[1].head().asNumber(), textRotation.head().asNumber(), textScale.head().asNumber()));
}

void NotebookApp::makeDiscretePlot() {


	//qDebug << exp;
	//exp.head().asString

	emit(discretePlotReady());


}