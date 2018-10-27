
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
	QObject::connect(this, SIGNAL(TextReady(QString, double , double)), output, SLOT(DisplayText(QString, double, double)));


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
		//out = "Error:Invalid Program. Could not parse.";
		// Send a Parse error to output
		//TextforOut = QString::fromStdString(out);
		//error("Could not open file for reading.");
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
	
	//std::string line;
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
			//outstream << exp;
			//out = outstream.str();
			//TextforOut = QString::fromStdString(out);
		}
		catch (const SemanticError& ex) {
			outstream  << ex.what();
			out = outstream.str();
			TextforOut = QString::fromStdString(out);
			emit ExpressionReady(TextforOut);
		}
		// send thhs the exp to the output now
		// or catch and error
	}
	//emit ExpressionReady(TextforOut);
}


// what are this 
//void NotebookApp::plotScriptEvaluated(QString output) {

//}
// This method will need to be used as the main loop inside plot script

void NotebookApp::plotScriptInputReady(QString input) {
		
	std::string line = input.toStdString();
	repl(line);
}

void NotebookApp::whatGoesWhere(Expression exp) {
	
	//interp.evaluate(argexp).isHeadList();
	// Regular Expression print to text 
	std::string propertyKey = "\"object-name\"";
	Expression objectName; 
	if (!exp.isPropertyListEmpty()) {
		objectName = exp.getPropertyList(propertyKey);
		if (objectName.isHeadString() )
		{
			// Draw point
			if (objectName.head().asString() == "\"point\""){
				Expression pointSize;
				//Expression 
				pointSize = exp.getPropertyList("\"size\"");

				auto xcor = exp.tailConstBegin();
				auto ycor = exp.tailConstEnd();
				std::vector<Expression> cordinates(xcor, ycor);
				Expression x = cordinates.front();
				Expression y = cordinates.back();
				emit(EllipseReady(x.head().asNumber(), y.head().asNumber(), pointSize.head().asNumber()));
			}
			// Draw line
			if (objectName.head().asString() == "\"line\"") {
				Expression lineThickness;
				lineThickness = exp.getPropertyList("\"thickness\"");
				
				auto point1 = exp.tailConstBegin();
				auto point2 = exp.tailConstEnd();
				std::vector<Expression> line(point1, point2);
				
				auto p1x = line[0].tailConstBegin();
				auto p1y = line[0].tailConstEnd();
				auto p2x = line[1].tailConstBegin();
				auto p2y = line[1].tailConstEnd();
				
				std::vector<Expression> p1(p1x, p1y);
				std::vector<Expression> p2(p2x, p2y);
				emit(LineReady(p1[0].head().asNumber(), p1[1].head().asNumber(), p2[0].head().asNumber(), p2[1].head().asNumber(), lineThickness.head().asNumber()));

			}
			if (objectName.head().asString() == "\"text\"") {
				Expression textPosition;
				textPosition = exp.getPropertyList("\"position\"");
				auto xcor = textPosition.tailConstBegin();
				auto ycor = textPosition.tailConstEnd();
				std::vector<Expression> cordinates(xcor, ycor);
				QString words = QString::fromStdString(exp.head().asString());

				emit(TextReady(words, cordinates[0].head().asNumber(), cordinates[1].head().asNumber()));
			}
		}
	}
	else
	{
		std::stringstream outstream;
		std::string out;
		QString TextforOut;
		outstream << exp;
		out = outstream.str();
		TextforOut = QString::fromStdString(out);
		emit ExpressionReady(TextforOut);
	}
	// 
	//exp.
}