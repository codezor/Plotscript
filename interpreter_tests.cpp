#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"

Expression run(const std::string & program){

  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parseStream(iss);
  if(!ok){
    std::cerr << "Failed to parse: " << program << std::endl;
  }
  REQUIRE(ok == true);

  Expression result;
  REQUIRE_NOTHROW(result = interp.evaluate());

  return result;
}

TEST_CASE( "Test Interpreter parser with expected input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == true);
}

TEST_CASE( "Test Interpreter parser with numerical literals", "[interpreter]" ) {

  std::vector<std::string> programs = {"(1)", "(+1)", "(+1e+0)", "(1e-0)"};

  for(auto program : programs){
    std::istringstream iss(program);

    Interpreter interp;

    bool ok = interp.parseStream(iss);

    REQUIRE(ok == true);
  }

  {
    std::istringstream iss("(define x 1abc)");

    Interpreter interp;

    bool ok = interp.parseStream(iss);

    REQUIRE(ok == false);
  }
}

TEST_CASE( "Test Interpreter parser with truncated input", "[interpreter]" ) {

  {
    std::string program = "(f";
    std::istringstream iss(program);

    Interpreter interp;
    bool ok = interp.parseStream(iss);
    REQUIRE(ok == false);
  }

  {
    std::string program = "(begin (define r 10) (* pi (* r r";
    std::istringstream iss(program);

    Interpreter interp;
    bool ok = interp.parseStream(iss);
    REQUIRE(ok == false);
  }
}

TEST_CASE( "Test Interpreter parser with extra input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r))) )";
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with single non-keyword", "[interpreter]" ) {

  std::string program = "hello";
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with empty input", "[interpreter]" ) {

  std::string program;
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with empty expression", "[interpreter]" ) {

  std::string program = "( )";
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with bad number string", "[interpreter]" ) {

  std::string program = "(1abc)";
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with incorrect input. Regression Test", "[interpreter]" ) {

  std::string program = "(+ 1 2) (+ 3 4)";
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter result with literal expressions", "[interpreter]" ) {

  { // Number
    std::string program = "(4)";
    Expression result = run(program);
    REQUIRE(result == Expression(4.));
  }

  { // Symbol
    std::string program = "(pi)";
    Expression result = run(program);
    REQUIRE(result == Expression(atan2(0, -1)));
  }

  { // Complex
	  std::string program = "(I)";
	  Expression result = run(program);
	  REQUIRE(result == Expression(std::complex<double>(0.0, 1.0)));
  }


}

TEST_CASE( "Test Interpreter result with simple procedures (add)", "[interpreter]" ) {

  { // add, binary case
    std::string program = "(+ 1 2)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(3.));
  }

  { // add, 3-ary case
    std::string program = "(+ 1 2 3)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(6.));
  }

  { // add, 6-ary case
    std::string program = "(+ 1 2 3 4 5 6)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(21.));
  }
}

TEST_CASE( "Test Interpreter special forms: begin and define", "[interpreter]" ) {

  {
    std::string program = "(define answer 42)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }

  {
    std::string program = "(begin (define answer 42)\n(answer))";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }

  {
    std::string program = "(begin (define answer (+ 9 11)) (answer))";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(20.));
  }

  {
    std::string program = "(begin (define a 1) (define b 1) (+ a b))";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(2.));
  }
}

TEST_CASE( "Test a medium-sized expression", "[interpreter]" ) {

  {
    std::string program = "(+ (+ 10 1) (+ 30 (+ 1 1)))";
    Expression result = run(program);
    REQUIRE(result == Expression(43.));
  }
}

TEST_CASE("Test a List expression", "[interpreter]") {

	{
		std::string program = "(append (list 10 1) (2))";
		INFO(program);
		Expression result = run(program);
		std::vector<Expression> answer= std::vector<Expression>();	
		answer.emplace_back(Atom(10.0));
		answer.emplace_back(Atom(1.0));	
		answer.emplace_back(Atom(2.0));		
		REQUIRE(result == answer);
	}
	{
		std::string program = "(join (list 10 1) (list 2))";
		INFO(program);
		Expression result = run(program);
		std::vector<Expression> answer = std::vector<Expression>();
		answer.emplace_back(Atom(10.0));
		answer.emplace_back(Atom(1.0));
		answer.emplace_back(Atom(2.0));
		REQUIRE(result == answer);
	}

	{
		std::string program = "(range 0 4 1)";
		INFO(program);
		Expression result = run(program);
		std::vector<Expression> answer = std::vector<Expression>();
		answer.emplace_back(Atom(0.0));
		answer.emplace_back(Atom(1.0));
		answer.emplace_back(Atom(2.0));
		answer.emplace_back(Atom(3.0));
		answer.emplace_back(Atom(4.0));
		REQUIRE(result == answer);
	}
}
TEST_CASE("Testing Lambda", "[interpreter]") {

	{
		std::string input = "(begin (define a 1) (define x 100)(define f (lambda (x) (begin (define b 12)(+ a b x))))(f 2))";
		INFO(input);
		Expression result = run(input);
		REQUIRE(result == Expression(15.0));
	}
	{
		std::string input = "(lambda (x))";

		Interpreter interp;

		std::istringstream iss(input);

		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		INFO(input);

		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
	}
	{
		std::string input = "(lambda (2) (+ 1))";

		Interpreter interp;

		std::istringstream iss(input);

		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		INFO(input);

		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
	}
}

TEST_CASE("Testing apply", "[interpreter]") 
{
	{
		std::string input = "(apply + (list 1 2 3 4))";
		INFO(input);
		Expression result = run(input);
		REQUIRE(result == Expression(10.0));
	}
	{
		std::string input = "(apply + 3)";

		Interpreter interp;

		std::istringstream iss(input);

		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		INFO(input);
		
		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError );
	}
	{
		std::string input = "(apply / (list 1 2 4))";

		Interpreter interp;

		std::istringstream iss(input);

		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		INFO(input);

		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
	}
	{
		std::string input = "(apply + )";

		Interpreter interp;

		std::istringstream iss(input);

		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		INFO(input);

		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
	}

	{
		std::string input = "(apply 3 3 )";

		Interpreter interp;

		std::istringstream iss(input);

		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		INFO(input);

		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
	}
}
TEST_CASE("Testing map", "[interpreter]")
{
	{
		std::string input = "(map / (list 1 2 4))";
		INFO(input);
		Expression result = run(input);
		Expression answer;
		answer.append(Atom(1.0));
		answer.append(Atom(0.5));
		answer.append(Atom(0.25));

		REQUIRE(result == Expression(answer));
	}

	{
		std::string input = "(map + 3)";

		Interpreter interp;

		std::istringstream iss(input);

		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		INFO(input);

		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
	}

	{
		std::string input = "(map +)";

		Interpreter interp;

		std::istringstream iss(input);

		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		INFO(input);

		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
	}
	{
		std::string input = "(map  3)";

		Interpreter interp;

		std::istringstream iss(input);

		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		INFO(input);

		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
	}


	{
		std::string input = "(begin(define addtwo (lambda (x y) (+ x y)))(map addtwo(list 1 2 3)))";
		
		Interpreter interp;

		std::istringstream iss(input);

		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		INFO(input);

		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
	}
}
TEST_CASE( "Test arithmetic procedures", "[interpreter]" ) {

  {
    std::vector<std::string> programs = {"(+ 1 -2)",
					 "(+ -3 1 1)",
					 "(- 1)",
					 "(- 1 2)",
					 "(* 1 -1)",
					 "(* 1 1 -1)",
					 "(/ -1 1)",
					 "(/ 1 -1)"};

    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(-1.));
    }
  }
}


TEST_CASE( "Test some semantically invalid expresions", "[interpreter]" ) {

  std::vector<std::string> programs = {"(@ none)", // so such procedure
				       "(- 1 1 2)", // too many arguments
				       "(define begin 1)", // redefine special form
				       "(define pi 3.14)"}; // redefine builtin symbol
    for(auto s : programs){
      Interpreter interp;

      std::istringstream iss(s);

      bool ok = interp.parseStream(iss);
      REQUIRE(ok == true);

      REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
    }
}

TEST_CASE( "Test for exceptions from semantically incorrect input", "[interpreter]" ) {

  std::string input = R"(
(+ 1 a)
)";

  Interpreter interp;

  std::istringstream iss(input);

  bool ok = interp.parseStream(iss);
  REQUIRE(ok == true);

  REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
}

TEST_CASE( "Test malformed define", "[interpreter]" ) {

    std::string input = R"(
(define a 1 2)
)";

  Interpreter interp;

  std::istringstream iss(input);

  bool ok = interp.parseStream(iss);
  REQUIRE(ok == true);

  REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
}

TEST_CASE( "Test using number as procedure", "[interpreter]" ) {
    std::string input = R"(
(1 2 3)
)";

  Interpreter interp;

  std::istringstream iss(input);

  bool ok = interp.parseStream(iss);
  REQUIRE(ok == true);

  REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
}

