#include "environment.hpp"

#include <cassert>
#include <cmath>

#include <complex>
#include <list>
#include <vector>
//#include<iostream>>

#include "environment.hpp"
#include "semantic_error.hpp"

/***********************************************************************
Helper Functions
**********************************************************************/

// predicate, the number of args is nargs
bool nargs_equal(const std::vector<Expression> &args, unsigned nargs)
{
	return args.size() == nargs;
}

void require_numeric(const Expression &e, const std::string function_name)
{
	if (!e.isHeadNumber() && !e.isHeadComplex())
	{
		throw SemanticError("Error in call to " + function_name + ", argument not a number");
	}
	//should make this a function of both numer and complex
}

// Is number 
// Is complex 

/***********************************************************************
Each of the functions below have the signature that corresponds to the
typedef'd Procedure function pointer.
**********************************************************************/

// the default procedure always returns an expresison of type None
Expression default_proc(const std::vector<Expression> &args)
{
	args.size(); // make compiler happy we used this parameter
	return Expression();
};

Expression add(const std::vector<Expression> &args)
{

	// check all aruments are numbers, while adding
	bool is_complex = false;
	std::complex<double> sum = std::complex<double>(0.0, 0.0);
	//For
	for (auto &arg : args)
	{
		require_numeric(arg, "add");
		std::complex<double> a;

		if (arg.isHeadNumber())
		{
			a = std::complex<double>(arg.head().asNumber(), 0.0);
		}
		else if (arg.isHeadComplex())
		{
			is_complex = true;
			a = arg.head().asComplex();
		}
		sum += a;
	}

	if (is_complex)
	{
		return Expression(sum);
	}
	return Expression(sum.real());
};

Expression mul(const std::vector<Expression> &args)
{

	bool is_complex = false;
	std::complex<double> product = std::complex<double>(1.0, 0.0);
	std::complex<double> temp = std::complex<double>(0.0, 0.0);

	for (auto &arg : args)
	{
		require_numeric(arg, "mul");

		std::complex<double> a;
		// must add pre- condiotions like is the result complex already
		if (arg.isHeadNumber())
		{
			if (is_complex)
			{
				temp = product;
			}
			else
			{
				temp = std::complex<double>(product.real(), 0.0);
			}
			a = std::complex<double>(arg.head().asNumber(), 0.0);
		}
		else if (arg.isHeadComplex())
		{

			if (!is_complex)
			{
				temp = std::complex<double>(product.real(), 0.0);
			}
			else
			{
				temp = product;
			}

			is_complex = true;
			a = arg.head().asComplex();
		}
		product = std::complex<double>(((a.real() * temp.real()) - (a.imag() * temp.imag())), ((a.imag() * temp.real()) + (a.real() * temp.imag())));
	}

	if (is_complex)
	{
		return Expression(product);
	}
	return Expression(product.real());
};

Expression subneg(const std::vector<Expression> &args)
{
	const Expression NEG = Expression(-1.0);

	require_numeric(args[0], "subneg");
	Expression x = args[0];

	if (nargs_equal(args, 1))
	{
		Expression y = NEG;
		return Expression(mul(std::vector<Expression>{y, x}));
	}
	else if (nargs_equal(args, 2))
	{
		require_numeric(args[1], "subneg");
		Expression y = mul(std::vector<Expression>{NEG, args[1]});
		return Expression(add(std::vector<Expression>{x, y}));
	}
	throw SemanticError("Error in call to subneg: invalid number of arguments.");
};

Expression div(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 2))
	{
		throw SemanticError("Error in call to division: invalid number of arguments.");
	}

	require_numeric(args[0], "div");
	require_numeric(args[1], "div");

	bool is_complex = false;
	std::complex<double> numerator;
	std::complex<double> divisor;

	if (args[0].isHeadNumber())
	{
		numerator = std::complex<double>(args[0].head().asNumber(), 0.0);
	}
	else if (args[0].isHeadComplex())
	{
		is_complex = true;
		numerator = args[0].head().asComplex();
	}

	if (args[1].isHeadNumber())
	{
		divisor = std::complex<double>(args[1].head().asNumber(), 0.0);
	}
	else if (args[1].isHeadComplex())
	{
		is_complex = true;
		divisor = args[1].head().asComplex();
	}

	double realAns = (numerator.real() * divisor.real() + numerator.imag() * divisor.imag()) / (std::pow(divisor.real(), 2) + std::pow(divisor.imag(), 2));
	double imagAns = (numerator.imag() * divisor.real() - numerator.real() * divisor.imag()) / (std::pow(divisor.real(), 2) + std::pow(divisor.imag(), 2));

	if (is_complex)
	{
		return Expression(std::complex<double>(realAns, imagAns));
	}
	return Expression(realAns);
};

// Sqrt Procedure : Square root procedure unary
Expression sqrt(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to sqrt: invalid number of arguments.");
	}
	require_numeric(args[0], "sqrt");

	bool is_complex = false;
	std::complex<double> result(0, 0);

	// preconditions
	if (nargs_equal(args, 1))
	{

		if (args[0].isHeadNumber())
		{

			if (args[0].head().asNumber() >= 0)
			{
				result = std::complex<double>(sqrt(args[0].head().asNumber()), 0.0);
			}
			else
			{
				result = std::sqrt(std::complex<double>(args[0].head().asNumber(), 0.0));
				is_complex = true;
			}
		}
		else if (args[0].isHeadComplex())
		{
			result = std::sqrt(args[0].head().asComplex());
			is_complex = true;
		}
	}

	if (is_complex)
	{
		return Expression(result);
	}
	return Expression(result.real());
};

// exponential Procedure: biniary procedure
Expression exp(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 2))
	{
		throw SemanticError("Error in call to exp: invalid number of arguments.");
	}
	require_numeric(args[0], "exp");
	require_numeric(args[1], "exp");

	bool is_complex = false;
	std::complex<double> result(0.0, 0.0);

	//preconditions
	if ((args[0].isHeadNumber()) && (args[1].isHeadNumber()))
	{
		result = std::complex<double>(std::pow(args[0].head().asNumber(), args[1].head().asNumber()), 0.0);
	}
	else if ((args[0].isHeadComplex()) && (args[1].isHeadComplex()))
	{
		result = std::pow(args[0].head().asComplex(), args[1].head().asComplex());
		is_complex = true;
	}
	else if ((args[0].isHeadComplex()) && (args[1].isHeadNumber()))
	{
		result = std::pow(args[0].head().asComplex(), args[1].head().asNumber());
		is_complex = true;
	}
	else if ((args[0].isHeadNumber()) && (args[1].isHeadComplex()))
	{
		result = std::pow(args[0].head().asNumber(), args[1].head().asComplex());
		is_complex = true;
	}

	if (is_complex)
	{
		return Expression(result);
	}
	return Expression(result.real());
};

// Natural Logarithm Procedure: Natural Log unary
Expression ln(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to ln: invalid number of arguments.");
	}
	require_numeric(args[0], "ln");

	double result = 0;

	// preconditions
	if (args[0].isHeadNumber())
	{
		if (args[0].head().asNumber() < 0)
		{
			throw SemanticError("Error in call to natural log: invalid argument.");
		}
		result = std::log(args[0].head().asNumber());
	}
	else
	{
		throw SemanticError("Error in call to natural log: invalid argument must be positive.");
	}
	return Expression(result);
};

// Sin Procedure:
Expression sin(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to sin: invalid number of arguments.");
	}
	require_numeric(args[0], "sin");

	double result = 0;

	//preconditions
	if (args[0].isHeadNumber())
	{
		result = std::sin(args[0].head().asNumber());
	}
	else
	{
		throw SemanticError("Error in call to sin: invalid argument.");
	}
	return Expression(result);
};

// Cos Procedure:
Expression cos(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to cos: invalid number of arguments.");
	}
	require_numeric(args[0], "cos");

	double result = 0;
	//preconditions
	if (args[0].isHeadNumber())
	{
		result = std::cos(args[0].head().asNumber());
	}
	else
	{
		throw SemanticError("Error in call to cos: invalid argument.");
	}
	return Expression(result);
};

// Tan Procedure:
Expression tan(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to tan: invalid number of arguments.");
	}
	require_numeric(args[0], "tan");

	double result = 0;
	//preconditions
	if (args[0].isHeadNumber())
	{
		result = std::tan(args[0].head().asNumber());
	}
	else
	{
		throw SemanticError("Error in call to tan: invalid argument.");
	}
	return Expression(result);
};

// real Procedure:
Expression real(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to real: invalid number of arguments.");
	}
	require_numeric(args[0], "real");

	double result = 0;

	//preconditions
	if (args[0].isHeadComplex())
	{
		result = args[0].head().asComplex().real();
	}
	else
	{
		throw SemanticError("Error in call to real: invalid argument : argument must be complex.");
	}
	return Expression(result);
};

// imag Procedure:
Expression imag(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to imag: invalid number of arguments.");
	}
	require_numeric(args[0], "imag");
	double result = 0;

	//preconditions
	if (args[0].isHeadComplex())
	{
		result = args[0].head().asComplex().imag();
	}
	else
	{
		throw SemanticError("Error in call to imag: invalid argument : argument must be complex.");
	}
	return Expression(result);
};

// mag Procedure:
Expression mag(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to mag: invalid number of arguments.");
	}
	require_numeric(args[0], "mag");

	double result = 0;

	//preconditions
	if (args[0].isHeadComplex())
	{
		result = abs(args[0].head().asComplex());
	}
	else
	{
		throw SemanticError("Error in call to mag: invalid argument : argument must be complex.");
	}
	return Expression(result);
};

// arg Procedure:
Expression arg(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to arg: invalid number of arguments.");
	}
	require_numeric(args[0], "arg");

	double result = 0;

	if (args[0].isHeadComplex())
	{
		result = arg(args[0].head().asComplex());
	}
	else
	{
		throw SemanticError("Error in call to arg: invalid argument : argument must be complex.");
	}
	return Expression(result);
};

// conj Procedure:
Expression conj(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to conj: invalid number of arguments.");
	}
	require_numeric(args[0], "conj");

	std::complex<double> result(0, 0);

	//preconditions
	if (args[0].isHeadComplex())
	{
		result = conj(args[0].head().asComplex());
	}
	else
	{
		throw SemanticError("Error in call to conj: invalid argument : argument must be complex.");
	}
	return Expression(result);
};
Expression list(const std::vector<Expression> &args)
{
	std::list<Expression> m_list(args.begin(),args.end());

	return Expression(m_list);
};

Expression first(const std::vector<Expression> &args) {
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to first: more than one argument in call to first.");
	}
	
	Expression list = args.front();
	
	
	auto begin = list.tailConstBegin();
	auto end = list.tailConstEnd();
	std::list<Expression> first(begin,end);
	if (begin != end)
	{
		return Expression(first.front());
		//return Expression(*args.front().tailConstBegin());
	}
	else {
		throw SemanticError("Error: argument to first is an empty list");
	}
};

Expression rest(const std::vector<Expression> &args) 
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to rest: invalid number of arguments.");
	}
	Expression list = args.front();
	//std::list<Expression> m_list(*args.begin(), *args.end());
	//std::list<Expression> rest;// = Expression(*m_list);
	auto begin = list.tailConstBegin();
	auto end = list.tailConstEnd();
	std::list<Expression>whole(begin, end);
	
	if ((list.head().asSymbol() != ""))
	{
		throw SemanticError("Error: not a list");

	}
	else 
	{
		++ begin;
		std::list<Expression>rest(begin, end);
		//std::list<Expression>rest =whol
		return Expression(rest);
	}
	
	
	
};

Expression length(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 1))
	{
		throw SemanticError("Error in call to length: invalid number of arguments.");
	}

	Expression list = args.front();	
	auto begin = list.tailConstBegin();
	auto end = list.tailConstEnd();
	//std::list<Expression>length(list.tailConstBegin(), list.tailConstEnd());
	
	if ((list.head().asSymbol() != ""))
	{
		throw SemanticError("Error: not a list");
	   
	}
	else if (begin == end) {
		std::list<Expression>length(begin, end);
		return Expression(length.size());
	}
	else
	{

		std::list<Expression>length(begin, end);
		return Expression(length.size());
		
	}

	//return Expression(length.size());

};

Expression append(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 2))
	{
		throw SemanticError("Error in call to append: invalid number of arguments.");
	}
	
	Expression listy = args.front();	
	auto begin = listy.tailConstBegin();
	auto end = listy.tailConstEnd();
	Expression arg_add = args[1];
	require_numeric(arg_add, "append");
	if ((listy.head().asSymbol() != ""))
	{
		throw SemanticError("Error: not a list");

	}
	else
	{
		std::list<Expression>og_list(begin, end);
		//std::list<Expression>frontList =
		//Expression y = list(std::vector<Expression> {arg_add});
		std::list<Expression>frontList = og_list;//.emplace_back(arg_add);
		frontList.emplace_back(arg_add);
		return Expression(frontList);
		
	}	
};

Expression join(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 2))
	{
		throw SemanticError("Error in call to join: invalid number of arguments.");
	}
	Expression list1 = args.front();
	auto begin1 = list1.tailConstBegin();
	auto end1 = list1.tailConstEnd();
	Expression list2 = args[1];
	auto begin2 = list2.tailConstBegin();
	auto end2 = list2.tailConstEnd();
	std::list<Expression>frontList(begin1, end1);
	std::list<Expression>backList(begin2, end2);
	
	if ((list1.head().asSymbol() != "") || (list2.head().asSymbol() != ""))
	{
		throw SemanticError("Error: argument to join not a list");
	}
	std::list<Expression>joinedList = frontList;
	joinedList.splice(joinedList.end(), backList);
	return Expression(joinedList);
};

Expression range(const std::vector<Expression> &args)
{
	if (!nargs_equal(args, 3))
	{
		throw SemanticError("Error in call to range: invalid number of arguments.");
	}
	//require_numeric(lowerBound, "range");
	//require_numeric(upperBound, "range");
	//require_numeric(increment, "range");
	double lowerBound = args[0].head().asNumber();
	double upperBound = args[1].head().asNumber();
	double increment = args[2].head().asNumber();

	std::list<Expression>rangeList;

	if (!(lowerBound < upperBound) || !(increment > 0)) {
		throw SemanticError("Error in call to range: Arguments not what they should be");
	}
	else {
		
		double i = lowerBound;
		while (i <= upperBound) {
			rangeList.emplace_back(Expression (i));
				i = i + increment;
		}
		return Expression(rangeList);
	}


};


const double PI = std::atan2(0, -1);
const double EXP = std::exp(1);
// Added the complex type
const std::complex<double> I = std::complex<double>(0.0, 1.0);

Environment::Environment()
{

	reset();
}

bool Environment::is_known(const Atom &sym) const
{
	if (!sym.isSymbol())
		return false;

	return envmap.find(sym.asSymbol()) != envmap.end();
}

bool Environment::is_exp(const Atom &sym) const
{
	if (!sym.isSymbol())
		return false;

	auto result = envmap.find(sym.asSymbol());
	return (result != envmap.end()) && ((result->first == "list") || (result->second.type == ExpressionType));
}

Expression Environment::get_exp(const Atom &sym) const
{

	Expression exp;

	if (sym.isSymbol())
	{
		auto result = envmap.find(sym.asSymbol());
		if ((result != envmap.end()) && (result->second.type == ExpressionType))
		{
			exp = result->second.exp;
		}
	}
	return exp;
}

void Environment::add_exp(const Atom &sym, const Expression &exp)
{

	if (!sym.isSymbol())
	{
		throw SemanticError("Attempt to add non-symbol to environment");
	}

	// error if overwriting symbol map
	if (envmap.find(sym.asSymbol()) != envmap.end())
	{
		throw SemanticError("Attempt to overwrite symbol in environemnt");
	}

	envmap.emplace(sym.asSymbol(), EnvResult(ExpressionType, exp));
}

bool Environment::is_proc(const Atom &sym) const
{
	if (!sym.isSymbol())
		return false;

	auto result = envmap.find(sym.asSymbol());
	return (result != envmap.end()) && (result->second.type == ProcedureType);
}

Procedure Environment::get_proc(const Atom &sym) const
{

	//Procedure proc = default_proc;

	if (sym.isSymbol())
	{
		auto result = envmap.find(sym.asSymbol());
		if ((result != envmap.end()) && (result->second.type == ProcedureType))
		{
			return result->second.proc;
		}
	}

	return default_proc;
}

/*
Reset the environment to the default state. First remove all entries and
then re-add the default ones.
 */
void Environment::reset()
{

	envmap.clear();

	// Built-In value of pi
	envmap.emplace("pi", EnvResult(ExpressionType, Expression(PI)));

	// Added Built in Eulers number
	envmap.emplace("e", EnvResult(ExpressionType, Expression(EXP)));

	// Add Built in Symbol I: Complex numer I =sqrt(-1) = 0 + i
	envmap.emplace("I", EnvResult(ExpressionType, Expression(I)));

	// Procedure: add;
	envmap.emplace("+", EnvResult(ProcedureType, add));

	// Procedure: subneg;
	envmap.emplace("-", EnvResult(ProcedureType, subneg));

	// Procedure: mul;
	envmap.emplace("*", EnvResult(ProcedureType, mul));

	// Procedure: div;
	envmap.emplace("/", EnvResult(ProcedureType, div));

	// Procedure: sqrt;
	envmap.emplace("sqrt", EnvResult(ProcedureType, sqrt));

	// Procedure: exp;
	envmap.emplace("^", EnvResult(ProcedureType, exp));

	// Procedure: ln;
	envmap.emplace("ln", EnvResult(ProcedureType, ln));

	// Procuedure: sin;
	envmap.emplace("sin", EnvResult(ProcedureType, sin));

	// Procuedure: cos;
	envmap.emplace("cos", EnvResult(ProcedureType, cos));

	// Procuedure: tan;
	envmap.emplace("tan", EnvResult(ProcedureType, tan));

	// Procedure: real;
	envmap.emplace("real", EnvResult(ProcedureType, real));

	// Procedure: imag;
	envmap.emplace("imag", EnvResult(ProcedureType, imag));

	// Procedure: mag;
	envmap.emplace("mag", EnvResult(ProcedureType, mag));

	// Procedure: arg;
	envmap.emplace("arg", EnvResult(ProcedureType, arg));

	// Procedure: conj;
	envmap.emplace("conj", EnvResult(ProcedureType, conj));

	// Procedure: list;
	envmap.emplace("list", EnvResult(ProcedureType, list));

	// Procedure first
	envmap.emplace("first", EnvResult(ProcedureType, first));

	// Procedure rest
	envmap.emplace("rest", EnvResult(ProcedureType, rest));
	
	// Procedure lengths
	envmap.emplace("length", EnvResult(ProcedureType, length));

	// Procedure append
	envmap.emplace("append", EnvResult(ProcedureType, append));

	// Procedure join
	envmap.emplace("join", EnvResult(ProcedureType, join));
	
	// Procedure range
	envmap.emplace("range", EnvResult(ProcedureType, range));

}
