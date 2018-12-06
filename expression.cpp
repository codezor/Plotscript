#include "expression.hpp"
#include "environment.hpp"
#include "semantic_error.hpp"
#include <iostream>
#include <list>
#include <sstream>
#include <utility>
#include <string>
#include <map>
#include <iomanip>

Expression::Expression() {
	
}

Expression::Expression(const Atom& a) 

{
  m_head = a;
  
}

// recursive copy includes property list
Expression::Expression(const Expression& a) 

{
  m_head = a.m_head;
  for (auto e : a.m_tail) {
    m_tail.push_back(e);
  }
  m_propertyList.insert(a.m_propertyList.begin(), a.m_propertyList.end());
}
// Constructor with property list
Expression::Expression(const Expression& a,std::map<std::string, Expression>& es)
	: Expression()
{
		
	m_head = a.m_head;
	for (auto e : a.m_tail) {
		m_tail.push_back(e);
	}	
	m_propertyList.insert(es.begin(), es.end());		
}

Expression
Expression::getPropertyList(std::string key) {
	//this->m_propertyList;
	//tree_view(" ");
	if (m_propertyList.count(key) > 0)
	{		
		Expression property = m_propertyList[key];
		return property;
	}
	
	else {
		std::string result = "NONE";
		return Expression(result);
	}
}

Expression Expression::setPropertyList(const Expression expression_to_add, std::map<std::string, Expression> property) const noexcept
{	
	// Expand property list or overwrite properties
	if (!expression_to_add.isPropertyListEmpty()) {
		// preserve existing property list
		std::map<std::string, Expression> temp_pl = expression_to_add.m_propertyList;		
		property.insert(temp_pl.begin(), temp_pl.end());
		
	}			
	return Expression(expression_to_add, property);
}
// TODO: pull redundent information
Expression Expression::setDiscretePlot(Expression DATA, Expression options)
{
	Expression Discrete;
	Discrete.m_head = Atom("list");
	Expression Points;
	Points.m_head = Atom("list");
	double xmin;
	double ymin;
	double xmax;
	double ymax;
	

	xmin = DATA.m_tail[0].m_tail[0].head().asNumber();
	ymin = DATA.m_tail[0].m_tail[1].head().asNumber();
	xmax = DATA.m_tail[0].m_tail[0].head().asNumber();
	ymax = DATA.m_tail[0].m_tail[1].head().asNumber();
	
	// (set-property "size" 0 (set-property "object-name" 
	for(auto e = DATA.tailConstBegin(); e != DATA.tailConstEnd(); ++e)
	{	
		// find x Max
		if(e->m_tail[0].head().asNumber() > xmax)
		{
			xmax = e->m_tail[0].head().asNumber();
		}
		// find xmin
		if(e->m_tail[0].head().asNumber() < xmin)
		{
			xmin = e->m_tail[0].head().asNumber();
		}
		// find ymax
		if(e->m_tail[1].head().asNumber() > ymax)
		{
			ymax = e->m_tail[1].head().asNumber();

		}
		//find ymin
		if(e->m_tail[1].head().asNumber() < ymin)
		{
			ymin = e->m_tail[1].head().asNumber();
		}		
	}	

	double scaleX = 20 / abs( xmax - xmin );
	double scaleY = -20 / abs( ymax - ymin );
	std::map<std::string, Expression> propsPoints;
	propsPoints["\"object-name\""] = Expression(Atom("\"point\""));
	propsPoints["\"size\""] = Expression(Atom(0.5));
	// make points
	for(auto e = DATA.tailConstBegin(); e != DATA.tailConstEnd(); ++e)
	{
		Expression temppoint;
		temppoint.m_head = Atom("list");
		
		if(e->m_tail[0].head().asNumber() < 0)
		{
			temppoint.m_tail.push_back(Expression(Atom(e->m_tail[0].head().asNumber() * scaleX)));
		}
		if(e->m_tail[0].head().asNumber() > 0)
		{
			temppoint.m_tail.push_back(Expression(Atom(e->m_tail[0].head().asNumber() * scaleX)));
		}
		if(e->m_tail[0].head().asNumber() == 0)
		{
			temppoint.m_tail.push_back(Expression(Atom(e->m_tail[0].head().asNumber())));
		}
		if(e->m_tail[1].head().asNumber() < 0)
		{
			temppoint.m_tail.push_back(Expression(Atom(e->m_tail[1].head().asNumber() * scaleY)));
		}
		if(e->m_tail[1].head().asNumber() > 0)
		{
			temppoint.m_tail.push_back(Expression(Atom(e->m_tail[1].head().asNumber() *scaleY)));

		}
		if(e->m_tail[1].head().asNumber() == 0)
		{
			temppoint.m_tail.push_back(Expression(Atom(e->m_tail[1].head().asNumber())));
		}
		temppoint = temppoint.setPropertyList(temppoint, propsPoints);
		Points.m_tail.push_back(temppoint);
	}	
	std::map<std::string, Expression> props;
	props["\"object-name\""] = Expression(Atom("\"line\""));
	props["\"thickness\""] = Expression(Atom(0));
	// make lollipop	
	for(auto e = Points.tailConstBegin(); e != Points.tailConstEnd(); ++e)
	{
		Expression Line;
		Line.m_head = Atom("list");		
		
		Expression temp;
		temp.m_head = Atom("list");
		// line drawn from ymax to point
		if(ymax < 0)
		{
			temp.m_tail.push_back(e->m_tail[0]);
			temp.m_tail.push_back(Expression(ymax*scaleY));
			Line.m_tail.push_back(*e);
			Line.m_tail.push_back(temp);
		}
		// Drawn up from y min
		else if(ymin > 0)
		{
			temp.m_tail.push_back(e->m_tail[0]);
			temp.m_tail.push_back(Expression(ymin*scaleY));
			Line.m_tail.push_back(*e);
			Line.m_tail.push_back(temp);
		}		
		// drawn from axis
		else
		{
			temp.m_tail.push_back(e->m_tail[0]);
			temp.m_tail.push_back(Expression(0.0));
			Line.m_tail.push_back(*e);
			Line.m_tail.push_back(temp);
		}
		Line = Line.setPropertyList(Line, props);
		Discrete.m_tail.push_back(*e);
		Discrete.m_tail.push_back(Line);
	}	
	Discrete.m_propertyList["\"object-name\""] = Expression(Atom("\"discrete-plot\""));
	Discrete= Discrete.PlotBoardersAndOptions(xmin, xmax, ymin, ymax, scaleX, scaleY, Discrete, options);
	return Discrete;
};

Expression Expression::PlotBoardersAndOptions(double xmin, double xmax, double ymin, double ymax , double scaleX, double scaleY, Expression Discrete, Expression options)
{	
	std::map<std::string, Expression> props;
	props["\"object-name\""] = Expression(Atom("\"line\""));
	props["\"thickness\""] = Expression(Atom(0));
	Expression xaxis;
	xaxis.m_head = Atom("list");

	Expression xaxisP1;
	Expression xaxisP2;
	xaxisP1.m_head = Atom("list");
	xaxisP1.m_head = Atom("list");

	Expression yaxis;
	yaxis.m_head = Atom("list");

	Expression yaxisP1;
	Expression yaxisP2;
	yaxisP1.m_head = Atom("list");
	yaxisP1.m_head = Atom("list");

	// if the x min > 0 then no axis should be drawn
	if(xmin > 0)
	{
		xaxisP1.m_tail.push_back(Expression(xmin*scaleX));
		xaxisP2.m_tail.push_back(Expression(xmin*scaleX));
	}
	// no axis should be drawn
	else if(xmax < 0)
	{
		xaxisP1.m_tail.push_back(Expression(xmax*scaleX));
		xaxisP2.m_tail.push_back(Expression(xmax*scaleX));
	}
	// The only time the x axis should be drawn
	else
	{
		xaxisP1.m_tail.push_back(Expression(0.0));
		xaxisP2.m_tail.push_back(Expression(0.0));

	}
	xaxisP1.m_tail.push_back(Expression(ymin*scaleY));
	xaxisP2.m_tail.push_back(Expression(ymax*scaleY));

	xaxis.m_tail.push_back(xaxisP1);
	xaxis.m_tail.push_back(xaxisP2);
	if(xmax >= 0 && xmin <= 0)
	{
		xaxis = xaxis.setPropertyList(xaxis, props);
	}
	yaxisP1.m_tail.push_back(Expression(xmin*scaleX));
	yaxisP2.m_tail.push_back(Expression(xmax*scaleX));
	if(ymin > 0)
	{
		yaxisP1.m_tail.push_back(Expression(ymin*scaleY));
		yaxisP2.m_tail.push_back(Expression(ymin*scaleY));
	}
	else if(ymax < 0)
	{
		yaxisP1.m_tail.push_back(Expression(ymax*scaleY));
		yaxisP2.m_tail.push_back(Expression(ymax*scaleY));
	}
	// The only time the y axis should be drawn
	else
	{
		yaxisP1.m_tail.push_back(Expression(0.0));
		yaxisP2.m_tail.push_back(Expression(0.0));

	}
	if(ymax >= 0 && ymin <= 0)
	{
		yaxis = yaxis.setPropertyList(yaxis, props);
	}
	yaxis.m_tail.push_back(yaxisP1);
	yaxis.m_tail.push_back(yaxisP2);

	// Drawing the boarders 
	Expression minXY;
	minXY.m_head = Atom("list");
	minXY.m_tail.push_back(Expression(Atom(xmin*scaleX)));
	minXY.m_tail.push_back(Expression(Atom(ymin*scaleY)));


	Expression maxXY;
	maxXY.m_head = Atom("list");
	maxXY.m_tail.push_back(Expression(Atom(xmax*scaleX)));
	maxXY.m_tail.push_back(Expression(Atom(ymax*scaleY)));

	Expression minXmaxY;
	minXmaxY.m_head = Atom("list");
	minXmaxY.m_tail.push_back(Expression(Atom(xmin*scaleX)));
	minXmaxY.m_tail.push_back(Expression(Atom(ymax*scaleY)));


	Expression maxXminY;
	maxXminY.m_head = Atom("list");
	maxXminY.m_tail.push_back(Expression(Atom(xmax *scaleX)));
	maxXminY.m_tail.push_back(Expression(Atom(ymin*scaleY)));


	Expression TopLine;
	TopLine.m_head = Atom("list");
	TopLine.m_tail.push_back(minXY);
	TopLine.m_tail.push_back(maxXminY);
	TopLine = TopLine.setPropertyList(TopLine, props);

	Expression BottomLine;
	BottomLine.m_head = Atom("list");
	BottomLine.m_tail.push_back(minXmaxY);
	BottomLine.m_tail.push_back(maxXY);
	BottomLine = BottomLine.setPropertyList(BottomLine, props);

	Expression RightLine;
	RightLine.m_head = Atom("list");
	RightLine.m_tail.push_back(minXY);
	RightLine.m_tail.push_back(minXmaxY);
	RightLine = RightLine.setPropertyList(RightLine, props);

	Expression LeftLine;
	LeftLine.m_head = Atom("list");
	LeftLine.m_tail.push_back(maxXY);
	LeftLine.m_tail.push_back(maxXminY);
	LeftLine = LeftLine.setPropertyList(LeftLine, props);

	// Insert Lines
	Discrete.m_tail.push_back(TopLine);
	Discrete.m_tail.push_back(BottomLine);
	Discrete.m_tail.push_back(RightLine);
	Discrete.m_tail.push_back(LeftLine);
	Discrete.m_tail.push_back(xaxis);
	Discrete.m_tail.push_back(yaxis);
	
	// push back scaled axis
	Discrete.m_tail.push_back(Expression(Atom("\"" + std::to_string(( int )xmin) + "\"")));
	Discrete.m_tail.push_back(Expression(Atom("\"" + std::to_string(( int )ymin) + "\"")));
	Discrete.m_tail.push_back(Expression(Atom("\"" + std::to_string(( int )xmax) + "\"")));
	Discrete.m_tail.push_back(Expression(Atom("\"" + std::to_string(( int )ymax) + "\"")));
	
	//Discrete.m_propertyList["\"object-name\""] = Expression(Atom("\"discrete-plot\""));
	Discrete.m_propertyList["\"xmin\""] = Expression(Atom(xmin));
	Discrete.m_propertyList["\"xmax\""] = Expression(Atom(xmax));
	Discrete.m_propertyList["\"ymin\""] = Expression(Atom(ymin));
	Discrete.m_propertyList["\"ymax\""] = Expression(Atom(ymax));

	std::map<std::string, Expression> propsD;
	for(auto pr = options.tailConstBegin(); pr != options.tailConstEnd(); ++pr)
	{
		// Make a property list
		Expression property = Expression(Atom(pr->m_tail[0].head().asString()));

		if(pr->m_tail[1].isHeadString())
		{
			Discrete.m_tail.push_back(Expression(Atom(pr->m_tail[1].head().asString())));
		}

		Discrete.m_propertyList[property.head().asString()] = Expression(Atom(pr->m_tail[1].head()));

	}

	Discrete = Discrete.setPropertyList(Discrete, propsD);

	
	return Discrete;
}

Expression Expression::setContinuousPlot(Environment&env)
{
	env.is_known(Atom());
	Expression Bounds = m_tail[1];
	Expression XRange = Bounds.MakeRange(Bounds);
	Expression results;
	results.m_head = Atom("map");
	results.m_tail.emplace_back(m_tail[0]);
	results.m_tail.emplace_back(XRange);
	Expression YRange = results.eval(env);
	Expression options = m_tail[2];


	Expression Lines;
	int n = XRange.m_tail.size();
	int m = YRange.m_tail.size();
	if(n != m)
	{
		std::stringstream s; s << __FILE__ << ":" << __LINE__; throw ( s.str() );
	}

	double xmin = Bounds.m_tail[0].head().asNumber();
	double xmax = Bounds.m_tail[1].head().asNumber();
	double ymin = YRange.m_tail[0].head().asNumber();
	double ymax = YRange.m_tail[0].head().asNumber();;
	Expression Continous;
	Continous.m_head = Atom("list");
	std::map<std::string, Expression> props;
	props["\"object-name\""] = Expression(Atom("\"line\""));
	props["\"thickness\""] = Expression(Atom(0));
	// this only makes points it needs to make lines
	Expression Datapoint;
	Datapoint.m_head = Atom("list");
	Lines.m_head = Atom("list");
	auto xi = XRange.tailConstBegin();
	auto yi = YRange.tailConstBegin();
	for(int i = 0; i < n; ++i)
	{
		std::vector<Expression> Point;
		Point.emplace_back(*xi);
		Point.emplace_back(*yi);
		Datapoint.m_tail.push_back(Expression(Point));
		if(yi->head().asNumber() < ymin)
		{
			ymin = yi->head().asNumber();
		}
		else if(yi->head().asNumber() > ymax)
		{
			ymax = yi->head().asNumber();
		}
		++xi;
		++yi;
	}

	double scaleX = 20.0 / abs(xmax - xmin);
	double scaleY = -20.0 / abs(ymax - ymin);
	
	auto xii = XRange.tailConstBegin();
	auto yii = YRange.tailConstBegin();
	//for(int i = 0; i < n; ++i)
	Expression Datapoints;
	Datapoints.m_head = Atom("list");
	for (auto po = Datapoint.tailConstBegin(); po != Datapoint.tailConstEnd(); ++po)
	{	
		std::vector<Expression> Points;
		Points.emplace_back(xii->head().asNumber()*scaleX);
		Points.emplace_back(yii->head().asNumber()*scaleY);
		Datapoints.m_tail.push_back(Expression(Points));
		//po->m_tail[0].head().asNumber()
		++xii;
		++yii;
	}
	
	auto pointIt = Datapoints.tailConstBegin();
	int count  = Datapoints.m_tail.size();
	int i = 0;
	while(pointIt != Datapoints.tailConstEnd() && i < (count- 1)  )
	{		
		Expression Line;
		Line.m_head = Atom("list");
		Line.m_tail.push_back(*pointIt);		
		++pointIt;
		Line.m_tail.push_back( *pointIt);
		Line = Line.setPropertyList(Line, props);
		Continous.m_tail.push_back(Line);
		//Lines.m_tail.push_back(Line);
		//Lines = Lines.setPropertyList(Lines, props);
		
		i++;
	}
	//Lines = Lines.setPropertyList(Lines, props);
	Continous.m_tail.push_back(Lines);
	
	Continous = Continous.PlotBoardersAndOptions(xmin, xmax, ymin, ymax, scaleX, scaleY, Continous, options);
	Continous.m_propertyList["\"object-name\""] = Expression(Atom("\"continuous-plot\""));
	// change this to not use discreet plot
	// return //Continous.setDiscretePlot(Datapoints, options));
	return Expression(Continous);
}

Expression Expression::MakeRange(Expression Bounds)
{
	double lowerBound = Bounds.m_tail[0].head().asNumber();
	double upperBound = Bounds.m_tail[1].head().asNumber();
	double DeltaX = (upperBound - lowerBound) / 51;
	Expression rangeList;
	rangeList.m_head = Atom("list");
	double i = lowerBound;
	while(i <= upperBound)
	{
		rangeList.m_tail.emplace_back(Expression(i));
		i = i + DeltaX;
	}
	return rangeList;
}
Expression::Expression(const std::vector<Expression>& es) : Expression()

{
	// maybe set the head to list and then modify list printing to ignore the "list" head?
  m_head = Atom("list");
 
  for (auto e : es) {
    m_tail.emplace_back(e);	   
  }
 
}

Expression&
Expression::operator=(const Expression& a)
{

  // prevent self-assignment
  if (this != &a) {
    m_head = a.m_head;
    m_tail.clear();
    for (auto e : a.m_tail) {
      m_tail.push_back(e);
    }
  }

  m_propertyList.insert(a.m_propertyList.begin(), a.m_propertyList.end());
  return *this;
}

Atom&
Expression::head()
{
  return m_head;
}

const Atom&
Expression::head() const
{
  return m_head;
}

bool
Expression::isHeadNumber() const noexcept
{
  return m_head.isNumber();
}

bool
Expression::isHeadComplex() const noexcept
{
  return m_head.isComplex();
}

bool
Expression::isHeadSymbol() const noexcept
{
  return m_head.isSymbol();
}

bool Expression::isHeadString() const noexcept
{
	return m_head.isString();
}

bool Expression::isHeadList() const noexcept
{
	return (m_head.isSymbol() && m_head.asSymbol() == "list");
}

bool Expression::isHeadLambda() const noexcept 
{
	return (m_head.isSymbol() && m_head.asSymbol() == "lambda");
}

bool Expression::isPropertyListEmpty() const noexcept {
	return (m_propertyList.size() == 0);
}
void
Expression::append(const Atom& a)
{
  m_tail.emplace_back(a);
}

Expression*
Expression::tail()
{
  Expression* ptr = nullptr;

  if (m_tail.size() > 0) {
    ptr = &m_tail.back();
  }

  return ptr;
}

Expression::ConstIteratorType
Expression::tailConstBegin() const noexcept
{
  return m_tail.cbegin();
}

Expression::ConstIteratorType
Expression::tailConstEnd() const noexcept
{
  return m_tail.cend();
}

Expression
apply(const Atom& op,
      const std::vector<Expression>& args,
		const Environment& env)
{

  // head must be a symbol
  if (!op.isSymbol()) {
    throw SemanticError("Error during evaluation: procedure name not symbol");
  }
  
  // must map to a proc
  if (!env.is_proc(op)) {
	  if (!env.is_exp(op)) {
		  throw SemanticError(
			  "Error during evaluation: symbol " + op.asString() + " does not name a procedure");
	  }
  }
  
  // map from symbol to proc
  Procedure proc = env.get_proc(op);

  // call proc with args
  Expression result = proc(args);
  return result;
}
//#include <iostream>
Expression
Expression::handle_lookup(const Atom& head, const Environment& env)
{

  if (head.isSymbol()) {
    if (env.is_exp(head)) {
      // LIST SHOULD NEVER GET HERE
		
      Expression e = env.get_exp(head);

      return e;

    } else {
      throw SemanticError("Error during evaluation: unknown symbol " + head.asString());
    }
  } else if (head.isNumber()) {
    return Expression(head);
  } else if (head.isComplex()) {
    return Expression(head);
  }
  else if (head.isString()) {
	  return Expression(head);
  }
  else {
    throw SemanticError(
      "Error during evaluation: Invalid type in terminal expression");
  }
}

Expression
Expression::handle_begin(Environment& env)
{

  if (m_tail.size() == 0) {
    throw SemanticError("Error during evaluation: zero arguments to begin");
  }

  // evaluate each arg from tail, return the last
  Expression result;
  for (Expression::IteratorType it = m_tail.begin(); it != m_tail.end(); ++it) {
    result = it->eval(env);
  }

  return result;
}

Expression
Expression::handle_define(Environment& env)
{
	
  // tail must have size 2 or error
  if (m_tail.size() != 2) {
    throw SemanticError(
      "Error during evaluation: invalid number of arguments to define");
  }

  // tail[0] must be symbol
  if (!m_tail[0].isHeadSymbol()) {
    throw SemanticError(
      "Error during evaluation: first argument to define not symbol");
  }

  // but tail[0] must not be a special-form or procedure
  std::string s = m_tail[0].head().asSymbol();
  if ((s == "define") || (s == "begin")) {
    throw SemanticError(
      "Error during evaluation: attempt to redefine a special-form");
  }

  if (env.is_proc(m_head)) {
    throw SemanticError(
      "Error during evaluation: attempt to redefine a built-in procedure");
  }

  // eval tail[1]
  Expression result = m_tail[1].eval(env);

  if (env.is_exp(m_head)) {
    throw SemanticError("Error during evaluation: attempt to redefine a previously defined symbol");
  }
 
  env.add_exp(m_tail[0].head(), result);

  return result;
}

Expression
Expression::handle_lambda(Environment& env)
{
  env.is_known(Atom());
  // tail must have size 2 or error
  if (m_tail.size() != 2) {
    throw SemanticError(
      "Error during evaluation: invalid number of arguments to lambda");
  }

  // tail[0] must be symbol
  if (!m_tail[0].isHeadSymbol()) {
    throw SemanticError(
      "Error during evaluation: first argument to lambda not symbol");
  }

  Expression Parameters;

  Parameters.m_tail.emplace_back(m_tail.front().head());
  for (auto it = m_tail[0].m_tail.cbegin(); it != m_tail[0].m_tail.cend();
       ++it) {
    Parameters.m_tail.emplace_back(*it);
  }

  Expression second;
  second = m_tail[1];

  Expression result;
  result.m_head = m_head;
  result.m_tail.emplace_back(Parameters);
  result.m_tail.emplace_back(second);


  return (result);
}

Expression Expression::store_lamba(Environment& env, Expression& original) {

	// create a local environment for the lambda expression
	// Store temperary to preserve the enviornment
 	Environment keptenv = env;
	Environment shadow= env;	
		
	// Get the lambda function parameters and matching expressions.
	std::vector<Expression> parameters = original.m_tail[0].m_tail;
	std::vector<Expression> expressions = m_tail;

	// For each (parameter, expression) pair.
	if (parameters.size() != expressions.size())
	{
		throw SemanticError("Error in call to procedure : invalid number of arguments.");
	}
	const std::size_t NUM_PARAMS = parameters.size();
	for (std::size_t i = 0; i < NUM_PARAMS; ++i) {
		Expression parameter = parameters[i];
		Expression expression = expressions[i];

		// define the parameter in the shadow environment.
		Expression define(Atom("define"));
		define.m_tail.push_back(parameter);
		define.m_tail.push_back(expression);

		// Define the parameters in the shadow environment.
		define.eval(shadow);
	}

	// Now actually shadow the parent environment with the local environment
	// we just created.
	shadow.Shadow(keptenv, shadow);
	// yo dawg, i heard you like lambdas ...
	// so i put some lambdas, in your lambdas !
	// TODO: why won't this work ????
	 // auto lambda = [&, parameters](Expression Parmeters)-> Expression
	 // {return expressions; }; std::cout << lambda(parameters).eval(*new
	 // Environment);
	Expression result;

	result.m_tail.push_back(original.m_tail[1].eval(shadow));
		
	// Lamda statment back into AST evlauation
	env = keptenv;
	return result.m_tail[0];
}

Expression
Expression::handle_apply(Environment& env) {
	
	env.is_known(Atom());
	// Check only two areguments
	if (m_tail.size() != 2) {
			throw SemanticError(
				"Error during evaluation: invalid number of arguments to apply");
	}	
		
	if (!m_tail[0].head().isSymbol()) {
		throw SemanticError("Error in apply firt argument is not a proceudure");
	}

	Atom procID = m_tail[0].head();
	Expression proc = m_tail[0];	
	if ((!env.is_proc(procID) && !env.is_exp(procID)) || (!proc.m_tail.empty())) {
		throw SemanticError("Error in apply firt argument is not a proceudure");
	}
	Expression list = m_tail[1].eval(env);
	m_tail[1] = list;

	if (!m_tail[1].isHeadList())
	{
		throw SemanticError("Error: second argument to apply not a list");		
	}

	Expression results;
	Expression input;
	input.m_head =procID;
	Expression args = m_tail[1];
	
	for (auto a = args.tailConstBegin(); a != args.tailConstEnd(); ++a)
	{		
		input.m_tail.emplace_back(*a);		
	}	
	results = input;
	
	// see if the evaluation will cause an error
	try { results.eval(env); }	
	catch(SemanticError  &e) {		
		// TODO: find a better way to do this
		const std::string ER = ("Error: during apply: ");		
		throw SemanticError(ER + e.what());
	}
	
	return Expression(results.eval(env));
};

Expression Expression::handle_map(Environment& env) {

	env.is_known(Atom());
	// Check only two areguments
	if (m_tail.size() != 2) {
		throw SemanticError(
			"Error during evaluation: invalid number of arguments to map");
	}

	if (!m_tail[0].head().isSymbol()) {
		throw SemanticError("Error in map firt argument is not a proceudure");
	}
	
	Atom var= m_tail[0].head();
	Expression proc = m_tail[0];
	

	if ((!env.is_proc(var) && !env.is_exp(var)) || (!proc.m_tail.empty())) {
		throw SemanticError("Error in map firt argument is not a proceudure");
	}

	

	if(env.is_exp(m_head))
	{
		throw SemanticError("Error during evaluation: attempt to redefine a previously defined symbol");
	}
	Expression list = m_tail[1].eval(env);	
	m_tail[1] = list;
	
	if( !list.isHeadList() )
	{
		throw SemanticError("Error: second argument to map not a list");
	}
	//}
	
	Expression results;
	Expression input;
	input.m_head = var;
	Expression args = m_tail[1].m_tail;
	//std::cout << "var " << var <<"proc "<<proc << m_tail <<std::endl;
	for (auto a = args.tailConstBegin(); a != args.tailConstEnd(); ++a)
	{
		input.m_tail.push_back(*a);
		// see if the evaluation will cause an error
		try {input.eval(env); }
		catch (const SemanticError  &e) {
		// TODO: find a better way to do this		
		//results
		const std::string ER = ("Error: during map: ");		
				
		input.m_tail.pop_back();	
	
		throw SemanticError(ER + e.what());
		
		}
		results.m_tail.emplace_back(input.eval(env));		
		input.m_tail.pop_back();
	}
	
	return Expression(results);
}


// this is a simple recursive version. the iterative version is more
// difficult with the ast data structure used (no parent pointer).
// this limits the practical depth of our AST
Expression
Expression::eval(Environment& env)
{	

	if (m_tail.empty() && m_head.asSymbol() != "list") {
    return handle_lookup(m_head, env);
  }
  // handle begin special-form
  else if (m_head.isSymbol() && m_head.asSymbol() == "begin") {
    return handle_begin(env);
  }
  // handle define special-form
  else if (m_head.isSymbol() && m_head.asSymbol() == "define") {
    return handle_define(env);
  }
  // handle lamda special-form
  else if (m_head.isSymbol() && m_head.asSymbol() == "lambda") {
    return handle_lambda(env);
  }
  else if (m_head.isSymbol() && m_head.asSymbol() == "apply") {
	  return handle_apply(env);
  }
  else if (m_head.isSymbol() && m_head.asSymbol() == "map") {
	  return handle_map(env);
  }
  else if(m_head.isSymbol() && m_head.asSymbol() == "continuous-plot")
  {
	  return setContinuousPlot(env);
  }
  // else attempt to treat as procedure
  else {
	//tree_view(" ");
    Expression originial = env.get_exp(m_head);
    if (originial.isHeadLambda()) {
		
	  return(store_lamba( env, originial));      
    }

    std::vector<Expression> results;
    for (Expression::IteratorType it = m_tail.begin(); it != m_tail.end();
         ++it) {
      results.push_back(it->eval(env));
    }
    return apply(m_head, results, env);
  }

}

std::ostream&
operator<<(std::ostream& out, const Expression& exp)
{
	const Atom atom = exp.head();
	const std::string actual = atom.asString();
	const std::string expected = "NONE";
	if (actual == expected) {
		out << exp.head();
		return out;
	}
  out << "(";
  if ((!exp.isHeadLambda()) && (!exp.isHeadList())) {
    out << exp.head(); 
  }
  for (auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e) {
  
    if (e != exp.tailConstBegin() ||
        (exp.isHeadSymbol() && !exp.isHeadList() &&
         !exp.isHeadLambda())) {
      out << " ";
    }
    out << *e;
  }

  out << ")";

  return out;
}

bool
Expression::operator==(const Expression& exp) const noexcept
{

  bool result = (m_head == exp.m_head);

  result = result && (m_tail.size() == exp.m_tail.size());

  if (result) {
    for (auto lefte = m_tail.begin(), righte = exp.m_tail.begin();
         (lefte != m_tail.end()) && (righte != exp.m_tail.end());
         ++lefte, ++righte) {
      result = result && (*lefte == *righte);
    }
  }

  return result;
}

bool
operator!=(const Expression& left, const Expression& right) noexcept
{

  return !(left == right);
}

 // Used for debugging it
/*void Expression::tree_view(std::string indent) const {
	std::cout<< indent + "  " << m_head << std::endl;
	if (m_tail.size() > 0) {
		
		for (auto e : m_tail) {
			e.tree_view(indent + "  ");
		}
	}
	
}*/