#include "catch.hpp"

#include "environment.hpp"
#include "semantic_error.hpp"

#include <cmath>

TEST_CASE( "Test default constructor", "[environment]" ) {

  Environment env;

  INFO("Built-in constants.");

  REQUIRE(env.is_known(Atom("pi")));
  REQUIRE(env.is_exp(Atom("pi")));

  REQUIRE(env.is_known(Atom("e")));
  REQUIRE(env.is_exp(Atom("e")));

  REQUIRE(env.is_known(Atom("I")));
  REQUIRE(env.is_exp(Atom("I")));

  REQUIRE(!env.is_known(Atom("hi")));
  REQUIRE(!env.is_exp(Atom("hi")));

  INFO("Built-in procedures");

  REQUIRE(env.is_proc(Atom("+")));
  REQUIRE(env.is_proc(Atom("-")));
  REQUIRE(env.is_proc(Atom("*")));
  REQUIRE(env.is_proc(Atom("/")));
  REQUIRE(env.is_proc(Atom("sqrt")));
  REQUIRE(env.is_proc(Atom("^")));
  REQUIRE(env.is_proc(Atom("ln")));
  REQUIRE(env.is_proc(Atom("sin")));
  REQUIRE(env.is_proc(Atom("cos")));
  REQUIRE(env.is_proc(Atom("tan")));
  REQUIRE(env.is_proc(Atom("real")));
  REQUIRE(env.is_proc(Atom("imag")));
  REQUIRE(env.is_proc(Atom("mag")));
  REQUIRE(env.is_proc(Atom("arg")));
  REQUIRE(env.is_proc(Atom("conj")));
  REQUIRE(!env.is_proc(Atom("op")));
}

TEST_CASE( "Test get expression", "[environment]" ) {
  Environment env;

  INFO("Built-in constants should have correct value when interpreter starts");

  REQUIRE(env.get_exp(Atom("pi")) == Expression(std::atan2(0, -1)));
  REQUIRE(env.get_exp(Atom("e")) == Expression(std::exp(1)));
  REQUIRE(env.get_exp(Atom("I")) == Expression(std::complex<double>(0.0,1.0)));
  REQUIRE(env.get_exp(Atom("hi")) == Expression());
}

TEST_CASE( "Test add expression", "[environment]" ) {
  Environment env;

  INFO("Retrieving a stored variable should give the original.");

  {
    Expression a(Atom(1.0));
    env.add_exp(Atom("one"), a);
    REQUIRE(env.is_known(Atom("one")));
    REQUIRE(env.is_exp(Atom("one")));
    REQUIRE(env.get_exp(Atom("one")) == a);
  }

  {
    Expression a(Atom("hello"));
    env.add_exp(Atom("hi"), a);
    REQUIRE(env.is_known(Atom("hi")));
    REQUIRE(env.is_exp(Atom("hi")));
    REQUIRE(env.get_exp(Atom("hi")) == a);
  }

  {
    Expression a(Atom(std::complex<double>(1.0,2.0)));
    env.add_exp(Atom("onetwo"), a);
    REQUIRE(env.is_known(Atom("onetwo")));
    REQUIRE(env.is_exp(Atom("onetwo")));
    REQUIRE(env.get_exp(Atom("onetwo")) == a);
  }

  {
    Atom s = Atom("s");
    Atom n = Atom(1.0);
    Atom c = Atom(std::complex<double>(2.0,3.0));

    INFO("Variable names should only be strings.");

    REQUIRE_THROWS_AS(env.add_exp(n, s), SemanticError);
    REQUIRE_THROWS_AS(env.add_exp(n, c), SemanticError);
    REQUIRE_THROWS_AS(env.add_exp(n, n), SemanticError);

    REQUIRE_THROWS_AS(env.add_exp(c, s), SemanticError);
    REQUIRE_THROWS_AS(env.add_exp(c, c), SemanticError);
    REQUIRE_THROWS_AS(env.add_exp(c, n), SemanticError);

    REQUIRE_NOTHROW(env.add_exp(Atom("a"), s));
    REQUIRE_NOTHROW(env.add_exp(Atom("b"), c));
    REQUIRE_NOTHROW(env.add_exp(Atom("c"), n));
  }
}

TEST_CASE( "Test get built-in procedure", "[environment]" ) {
  Environment env;

  INFO("Procedures that do not exist should return the default procedure.");

  Procedure p1 = env.get_proc(Atom("doesnotexist"));
  Procedure p2 = env.get_proc(Atom("alsodoesnotexist"));
  REQUIRE(p1 == p2);

  INFO("Calling a procedure that does not exist should return the default expression.");

  std::vector<Expression> args;
  REQUIRE(p1(args) == Expression());
  REQUIRE(p2(args) == Expression());

  INFO("add procedure")

  {
    Procedure padd = env.get_proc(Atom("+"));
    args.emplace_back(1.0);
    args.emplace_back(std::complex<double>(3.0, 8.0));
    REQUIRE(padd(args) == Expression(std::complex<double>(4.0, 8.0)));
  }
}
TEST_CASE("Test add 1 arg", "[environment]"){
  Environment env;
  std::vector<Expression> args;

  INFO("add procedure")

  {
    Procedure p = env.get_proc(Atom("+"));
    args.emplace_back(std::complex<double>(3.0, 8.0));
    REQUIRE(p(args) == Expression(std::complex<double>(3.0, 8.0)));
  }

}
TEST_CASE("Test subneg mixed type", "[environment]"){
  Environment env;
  std::vector<Expression> args;

  INFO("subneg procedure")

  {
    Procedure p = env.get_proc(Atom("-"));
    args.emplace_back(1.0);
    args.emplace_back(std::complex<double>(3.0, 8.0));
    REQUIRE(p(args) == Expression(std::complex<double>(-2.0, -8.0)));
  }
}
TEST_CASE("Test subneg complex", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("-"));
    args.emplace_back(std::complex<double>(3.0, 8.0));
    REQUIRE(p(args) == Expression(std::complex<double>(-3.0, -8.0)));
  }
}
TEST_CASE("Test subneg double", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("-"));
    args.emplace_back(-7.0);
    REQUIRE(p(args) == Expression(7.0));
  }
}
TEST_CASE("Test mul double zero", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  INFO("mul procedure");

  {
    Procedure p = env.get_proc(Atom("*"));
    args.emplace_back(1.0);
    args.emplace_back(0.0);
    REQUIRE(p(args) == Expression(0.0));
  }
}
TEST_CASE("Test mul double Idenity", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("*"));
    args.emplace_back(1.0);
    args.emplace_back(7.0);
    REQUIRE(p(args) == Expression(7.0));
  }
}

TEST_CASE("Test mul double", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("*"));
    args.emplace_back(std::complex<double>(1.0,2.0));
    args.emplace_back(8.0);
    REQUIRE(p(args) == Expression(std::complex<double>(8.0,16.0)));
  }
}
TEST_CASE("Test mul double negaitive", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("*"));
    args.emplace_back(-2.0);
    args.emplace_back(3.0);
    REQUIRE(p(args) == Expression(-6.0));
  }
}
TEST_CASE("Test div double zero", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  INFO("div procedure");

  {
    Procedure p = env.get_proc(Atom("/"));
    args.emplace_back(1.0);
    args.emplace_back(0.0);
    // divide by zero ???
  }
}
TEST_CASE("Test div double Idenity", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("/"));
    args.emplace_back(7.0);
    args.emplace_back(1.0);
    REQUIRE(p(args) == Expression(7.0));
  }
}

TEST_CASE("Test div double", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("/"));
    args.emplace_back(8.0);
    args.emplace_back(4.0);
    REQUIRE(p(args) == Expression(2.0));
  }
}
TEST_CASE("Test div double negitive", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("/"));
    args.emplace_back(-2.0);
    args.emplace_back(0.5);
    REQUIRE(p(args) == Expression(-4.0));
  }
}

TEST_CASE("Test sqrt double", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  INFO("sqrt procedure");

  {
    Procedure p = env.get_proc(Atom("sqrt"));
    args.emplace_back(4.0);
    REQUIRE(p(args) == Expression(2.0));
  }
}
TEST_CASE("Test sqrt double negitive", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("sqrt"));
    args.emplace_back(-4.0);
    // complex
  }
}
TEST_CASE("Test exp double double", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  INFO("exp procedure");

  {
    Procedure p = env.get_proc(Atom("^"));
    args.emplace_back(4.0);
    args.emplace_back(2.0);
    REQUIRE(p(args) == Expression(16.0));
  }
}
  TEST_CASE("Test ln", "[environment]"){
  Environment env;
  std::vector<Expression> args;

  INFO("ln procedure");
 }
TEST_CASE("Test sin", "[environment]"){
  Environment env;
  std::vector<Expression> args;

  INFO("sin procedure");
  {

  }
}

TEST_CASE("Test cos", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  INFO("cos procedure");
  {}
}
TEST_CASE("Test tan", "[environment]"){
  Environment env;
  std::vector<Expression> args;

  INFO("tan procedure");
  {

  }
}

TEST_CASE("Test real", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  INFO("real procedure");
  {}
}
TEST_CASE("Test imag", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  INFO("imag procedure");
  {}
}
TEST_CASE("Test mag", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  INFO("mag procedure");
  {}
}
TEST_CASE("Test arg", "[environment]"){
  Environment env;
  std::vector<Expression> args;

  INFO("arg procedure");
  {}
}
TEST_CASE("Test conj", "[environment]"){
  Environment env;
  std::vector<Expression> args;
  INFO("conj procedure");
  {}
}


TEST_CASE( "Test reset", "[environment]" ) {
  Environment env;

  Expression a(Atom(1.0));
  env.add_exp(Atom("one"), a);
  Expression b(Atom("hello"));
  env.add_exp(Atom("hi"), b);

  env.reset();
  REQUIRE(!env.is_known(Atom("one")));
  REQUIRE(!env.is_exp(Atom("one")));
  REQUIRE(env.get_exp(Atom("one")) == Expression());
  REQUIRE(!env.is_known(Atom("hi")));
  REQUIRE(!env.is_exp(Atom("hi")));
  REQUIRE(env.get_exp(Atom("hi")) == Expression());
}

TEST_CASE( "Test semeantic errors", "[environment]" ) {

  Environment env;

  {
    Expression exp(Atom("begin"));

    REQUIRE_THROWS_AS(exp.eval(env), SemanticError);
  }

  {
    Expression exp (Atom(" "));
    REQUIRE_THROWS_AS(exp.eval(env), SemanticError);
  }
}
