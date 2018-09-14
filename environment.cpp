#include "environment.hpp"

#include <cassert>
#include <cmath>

#include <complex>

#include "environment.hpp"
#include "semantic_error.hpp"

/***********************************************************************
Helper Functions
**********************************************************************/

// predicate, the number of args is nargs
bool nargs_equal(const std::vector<Expression> & args, unsigned nargs){
  return args.size() == nargs;
}

/***********************************************************************
Each of the functions below have the signature that corresponds to the
typedef'd Procedure function pointer.
**********************************************************************/

// the default procedure always returns an expresison of type None
Expression default_proc(const std::vector<Expression> & args){
  args.size(); // make compiler happy we used this parameter
  return Expression();
};

Expression add(const std::vector<Expression> & args){

  // check all aruments are numbers, while adding
  double result = 0.0;
  double imagResult = 0.0;
  bool complexPrint = false;
  // maybe add the real bits together here
  std::complex<double>complexResult=std::complex<double>(0.0,0.0);
  for( auto & a :args){
    if(a.isHeadNumber()){
      //this will be the result of real numbers
      result += a.head().asNumber();
    }
    // Logic needs to be added some where to determine if there is a complex number involved in addition
    else if (a.isHeadComplex()){
        // Add the real results
        result += a.head().asComplex().real();

        // Add the imaginary results
        imagResult += a.head().asComplex().imag();
        complexPrint = true;
    }
    else{
      throw SemanticError("Error in call to add, argument not a number");
    }
  }
  // if the result is complex return a complex number.
  if (complexPrint){
      // set the complex results
      complexResult = std::complex<double>(result,imagResult);
      return Expression(complexResult);
    }
  // if the results are doubles return the results asNumber
  else{
    return Expression(result);
    }
};

Expression mul(const std::vector<Expression> & args){

  double result = 1;
  double imagResult = 0;
  std::complex<double>complexResult=std::complex<double>(0.0,0.0);
  bool print = false;

  for( auto & a :args){
    // Pre-conditions check if the previous result is number is complex or not
    if (abs(imagResult) <= 0){
      if(a.isHeadNumber()){
        imagResult =imagResult* result;
        result *= a.head().asNumber();
        complexResult= std::complex<double>(result, imagResult);
      }
      //If complex then its a complex times a number here
      else if(a.isHeadComplex()){
        double tempReal = result *a.head().asComplex().real()  - imagResult *a.head().asComplex().imag();
        double tempImag = imagResult *a.head().asComplex().real() + result *a.head().asComplex().imag();
        result = tempReal;
        imagResult = tempImag;
        complexResult=std::complex<double>(result,imagResult);
        print =true;
      }
    }

    // If the current result is a complex the multiplication must be done  diffrently
    else if (abs (imagResult) > 0){
    print = true;
      if(a.isHeadNumber()){
        double tempReal = complexResult.real() *a.head().asNumber();
        double tempImag = complexResult.imag() *a.head().asNumber();
        result = tempReal;
        imagResult = tempImag;
        complexResult= std::complex<double> (tempReal,tempImag);
      }

      else if(a.isHeadComplex()){
       double tempReal = complexResult.real() *a.head().asComplex().real() - complexResult.imag() *a.head().asComplex().imag();
       double tempImag = complexResult.imag() *a.head().asComplex().real() + complexResult.real() *a.head().asComplex().imag();
       result = tempReal;
       imagResult = tempImag;
       complexResult= std::complex<double> (tempReal,tempImag);
      }
    }
    else{
      throw SemanticError("Error in call to mul, argument not a number");
    }
  }
  if (print){
    return Expression(complexResult);
  }
  else{
    return Expression(result);
  }
};


Expression subneg(const std::vector<Expression> & args){

  double result = 0;
  std::complex<double>complexResult=std::complex<double>(0.0,0.0);

  // preconditions
  if(nargs_equal(args,1)){
    if(args[0].isHeadNumber())
    {
      result = -args[0].head().asNumber();
      return Expression(result);
    }

    else if(args[0].isHeadComplex())
    {
      complexResult = -args[0].head().asComplex();
      return Expression(complexResult);
    }

    else{
      throw SemanticError("Error in call to negate: invalid argument.");
    }
  }
  else if(nargs_equal(args,2)){
    // Two real numbers
    if( (args[0].isHeadNumber()) && (args[1].isHeadNumber()) ){
      result = args[0].head().asNumber() - args[1].head().asNumber();
      return Expression(result);
    }
    // Two Complex numbers
    else if(args[0].isHeadComplex() && (args[1].isHeadComplex()))
    {
      complexResult = args[0].head().asComplex()-args[1].head().asComplex();
      return Expression(complexResult);
    }
    // First number is real and the second is complex
    else if (args[0].isHeadNumber() && (args[1].isHeadComplex()))
    {
      complexResult = args[0].head().asNumber()-args[1].head().asComplex();
      return Expression(complexResult);
    }
    // First Number is Imaginenary number and the second is real
    else if (args[1].isHeadNumber() && (args[0].isHeadComplex()))
    {
      complexResult =args[0].head().asComplex()-args[1].head().asNumber();
      return Expression(complexResult);
    }
    else{
      throw SemanticError("Error in call to subtraction: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to subtraction or negation: invalid number of arguments.");
  }
};

Expression div(const std::vector<Expression> & args){

  double result = 0;
  std::complex<double>complexResult=std::complex<double>(0.0,0.0);
  if(nargs_equal(args,2)){
    if( (args[0].isHeadNumber()) && (args[1].isHeadNumber()) ){
      result = args[0].head().asNumber() / args[1].head().asNumber();
      return Expression(result);
    }
    else if((args[0].isHeadComplex()) && (args[1].isHeadComplex())){
      complexResult = args[0].head().asComplex() / args[1].head().asComplex();
      return Expression(complexResult);
    }
    else if((args[0].isHeadNumber()) && (args[1].isHeadComplex())){
      complexResult = args[0].head().asNumber() / args[1].head().asComplex();
      return Expression(complexResult);
    }
    else if((args[0].isHeadComplex()) && (args[1].isHeadNumber())){
      complexResult = args[0].head().asComplex() / args[1].head().asNumber();
      return Expression(complexResult);
    }
    else{
      throw SemanticError("Error in call to division: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to division: invalid number of arguments.");
  }

};
// Sqrt Procedure : Square root procedure unary
Expression sqrt(const std::vector<Expression> & args ) {

  double result = 0;
  std::complex<double> complexResult (0,0);
  // preconditions
  if (nargs_equal(args,1)){

    if (args[0].isHeadNumber()){

      if(args[0].head().asNumber() >= 0 ){
        result =  sqrt(args[0].head().asNumber());
        return Expression(result);
      }

      else {
        complexResult = std::sqrt(std::complex<double> (args[0].head().asNumber(), 0.0));
        return Expression(complexResult);
      }
    }
    else if (args[0].isHeadComplex()){
      complexResult = std::sqrt(args[0].head().asComplex());
      return Expression(complexResult);
    }
    else {
      throw SemanticError("Error in call to square root: invalid argument.");
    }
  }
  else {
    throw SemanticError("Error in call to square root: invalid number of arguments.");
  }
};
// exponential Procedure: biniary procedure
Expression exp(const std::vector<Expression> & args){

  double result = 0;
  std::complex<double> complexResult(0.0,0.0);
  //preconditions
  if(nargs_equal(args,2)){
    if( (args[0].isHeadNumber()) && (args[1].isHeadNumber())) {
      result = std::pow(args[0].head().asNumber(), args[1].head().asNumber());
       return Expression(result);
    }
    else if ((args[0].isHeadComplex()) && (args[1].isHeadComplex())){
       complexResult =std::pow(args[0].head().asComplex(), args[1].head().asComplex());
       return Expression(complexResult);
    }
    else if((args[0].isHeadComplex()) && (args[1].isHeadNumber())){
       complexResult =std::pow(args[0].head().asComplex(), args[1].head().asNumber());
       return Expression(complexResult);
    }
    else if((args[0].isHeadNumber()) && (args[1].isHeadComplex())){
       complexResult =std::pow(args[0].head().asNumber(), args[1].head().asComplex());
       return Expression(complexResult);
    }
    else{
      throw SemanticError("Error in call to exponential: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to exponential: invalid number of arguments.");
  }
};

// Natural Logarithm Procedure: Natural Log unary
Expression ln(const std::vector<Expression> & args){

  double result = 0;

  // preconditions
  if(nargs_equal(args,1)){       //TODO: This structure is repeated manytimes should these checks be turned into a general function
    if(args[0].isHeadNumber()){
      if (args[0].head().asNumber() < 0){
        throw SemanticError("Error in call to natural log: invalid argument.");
      }
      result = std::log(args[0].head().asNumber());
    }
    else{
      throw SemanticError("Error in call to natural log: invalid argument must be positive.");
    }
  }
  else{
    throw SemanticError("Error in call to natural log: invalid number of arguments.");
  }
  return Expression(result);
};

// Sin Procedure:
Expression sin(const std::vector<Expression> & args){
  double result =0;

  //preconditions
  if(nargs_equal(args,1)){

    if(args[0].isHeadNumber()){
      result = std::sin(args[0].head().asNumber());
    }
    else{
      throw SemanticError("Error in call to sin: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to sin: invalid number of arguments.");
  }
  return Expression(result);

};

// Cos Procedure:
Expression cos(const std::vector<Expression> & args){
  double result =0;

  //preconditions
  if(nargs_equal(args,1)){

    if(args[0].isHeadNumber()){
      result = std::cos(args[0].head().asNumber());
    }
    else{
      throw SemanticError("Error in call to cos: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to cose: invalid number of arguments.");
  }
  return Expression(result);

};

// Tan Procedure:
Expression tan(const std::vector<Expression> & args){
  double result = 0;

  //preconditions
  if(nargs_equal(args,1)){

    if(args[0].isHeadNumber()){
      result = std::tan(args[0].head().asNumber());
    }
    else{
      throw SemanticError("Error in call to tan: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to tan: invalid number of arguments.");
  }
  return Expression(result);

};

// real Procedure:
Expression real(const std::vector<Expression> & args){
   double result = 0;

  //preconditions
  if(nargs_equal(args,1)){

    if(args[0].isHeadComplex()){
      result = args[0].head().asComplex().real();
    }
    else{
      throw SemanticError("Error in call to real: invalid argument : argument must be complex.");
    }
  }
  else{
    throw SemanticError("Error in call to real: invalid number of arguments.");
  }
  return Expression(result);

};
// imag Procedure:
Expression imag(const std::vector<Expression> & args){
   double result =0;

  //preconditions
  if(nargs_equal(args,1)){

    if(args[0].isHeadComplex()){
      result = args[0].head().asComplex().imag();
    }
    else{
      throw SemanticError("Error in call to imag: invalid argument : argument must be complex.");
    }
  }
  else{
    throw SemanticError("Error in call to imag: invalid number of arguments.");
  }
  return Expression(result);

};
// mag Procedure:
Expression mag(const std::vector<Expression> & args){
   double result =0;

  //preconditions
  if(nargs_equal(args,1)){

    if(args[0].isHeadComplex()){
      result = abs(args[0].head().asComplex());
    }
    else{
      throw SemanticError("Error in call to mag: invalid argument : argument must be complex.");
    }
  }
  else{
    throw SemanticError("Error in call to mag: invalid number of arguments.");
  }
  return Expression(result);

};
// arg Procedure:
Expression arg(const std::vector<Expression> & args){
   double result =0;

  //preconditions
  if(nargs_equal(args,1)){

    if(args[0].isHeadComplex()){
      result = arg(args[0].head().asComplex());
    }
    else{
      throw SemanticError("Error in call to arg: invalid argument : argument must be complex.");
    }
  }
  else{
    throw SemanticError("Error in call to arg: invalid number of arguments.");
  }
  return Expression(result);

};

// conj Procedure:
Expression conj(const std::vector<Expression> & args){
  std::complex<double>result (0,0);

  //preconditions
  if(nargs_equal(args,1)){

    if(args[0].isHeadComplex()){
      result = conj(args[0].head().asComplex());
    }
    else{
      throw SemanticError("Error in call to conj: invalid argument : argument must be complex.");
    }
  }
  else{
    throw SemanticError("Error in call to conj: invalid number of arguments.");
  }
  return Expression(result);

};


const double PI = std::atan2(0, -1);
const double EXP = std::exp(1);
// Added the complex type
const std::complex<double> I = std::complex<double>(0.0,1.0);

Environment::Environment(){

  reset();
}

bool Environment::is_known(const Atom & sym) const{
  if(!sym.isSymbol()) return false;

  return envmap.find(sym.asSymbol()) != envmap.end();
}

bool Environment::is_exp(const Atom & sym) const{
  if(!sym.isSymbol()) return false;

  auto result = envmap.find(sym.asSymbol());
  return (result != envmap.end()) && (result->second.type == ExpressionType);
}

Expression Environment::get_exp(const Atom & sym) const{

  Expression exp;

  if(sym.isSymbol()){
    auto result = envmap.find(sym.asSymbol());
    if((result != envmap.end()) && (result->second.type == ExpressionType)){
      exp = result->second.exp;
    }
  }

  return exp;
}

void Environment::add_exp(const Atom & sym, const Expression & exp){

  if(!sym.isSymbol()){
    throw SemanticError("Attempt to add non-symbol to environment");
  }

  // error if overwriting symbol map
  if(envmap.find(sym.asSymbol()) != envmap.end()){
    throw SemanticError("Attempt to overwrite symbol in environemnt");
  }

  envmap.emplace(sym.asSymbol(), EnvResult(ExpressionType, exp));
}

bool Environment::is_proc(const Atom & sym) const{
  if(!sym.isSymbol()) return false;

  auto result = envmap.find(sym.asSymbol());
  return (result != envmap.end()) && (result->second.type == ProcedureType);
}

Procedure Environment::get_proc(const Atom & sym) const{

  //Procedure proc = default_proc;

  if(sym.isSymbol()){
    auto result = envmap.find(sym.asSymbol());
    if((result != envmap.end()) && (result->second.type == ProcedureType)){
      return result->second.proc;
    }
  }

  return default_proc;
}

/*
Reset the environment to the default state. First remove all entries and
then re-add the default ones.
 */
void Environment::reset(){

  envmap.clear();

  // Built-In value of pi
  envmap.emplace("pi", EnvResult(ExpressionType, Expression(PI)));

  // Added Built in Eulers number
  envmap.emplace("e", EnvResult(ExpressionType, Expression(EXP)));

  // Add Built in Symbol I: Complex numer I =sqrt(-1) = 0 + i
  envmap.emplace("I", EnvResult(ExpressionType, Expression(I)));

  //envmap.emplace("I", EnvResult(ProcedureType, Icomplex));

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


}
