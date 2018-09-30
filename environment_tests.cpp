#include "catch.hpp"

#include "environment.hpp"
#include "semantic_error.hpp"

#include <cmath>

TEST_CASE("Test default constructor", "[environment]")
{

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
  REQUIRE(env.is_proc(Atom("list")));
  REQUIRE(env.is_proc(Atom("first")));
  REQUIRE(env.is_proc(Atom("rest")));
  REQUIRE(env.is_proc(Atom("length")));
  REQUIRE(env.is_proc(Atom("append")));
  REQUIRE(env.is_proc(Atom("join")));
  REQUIRE(env.is_proc(Atom("range")));
  REQUIRE(env.is_proc(Atom("range")));

  REQUIRE(!env.is_proc(Atom("op")));

}

TEST_CASE("Test get expression", "[environment]")
{
  Environment env;

  INFO("Built-in constants should have correct value when interpreter starts");

  REQUIRE(env.get_exp(Atom("pi")) == Expression(std::atan2(0, -1)));
  REQUIRE(env.get_exp(Atom("e")) == Expression(std::exp(1)));
  REQUIRE(env.get_exp(Atom("I")) == Expression(std::complex<double>(0.0, 1.0)));
  REQUIRE(env.get_exp(Atom("hi")) == Expression());
}

TEST_CASE("Test add expression", "[environment]")
{
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
    Expression a(Atom(std::complex<double>(1.0, 2.0)));
    env.add_exp(Atom("onetwo"), a);
    REQUIRE(env.is_known(Atom("onetwo")));
    REQUIRE(env.is_exp(Atom("onetwo")));
    REQUIRE(env.get_exp(Atom("onetwo")) == a);
  }

  {
    Atom s = Atom("s");
    Atom n = Atom(1.0);
    Atom c = Atom(std::complex<double>(2.0, 3.0));

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

TEST_CASE("Test get built-in procedure", "[environment]")
{
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
TEST_CASE("Test add arg", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  //INFO("add no arguments")
  //{
  //Procedure p = env.get_proc(Atom("+"));
  //REQUIRE_THROWS_AS(p(args), SemanticError);
  //args.emplace_back(std::complex<double>(3.0, 8.0));
  //REQUIRE(p(args) == Expression(std::complex<double>(3.0, 8.0)));
  //}
  INFO("add procedure")

  {
    Procedure p = env.get_proc(Atom("+"));
    args.emplace_back(std::complex<double>(3.0, 8.0));
    REQUIRE(p(args) == Expression(std::complex<double>(3.0, 8.0)));
  }
}

TEST_CASE("Test subneg mixed type", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  //INFO ("subneg procedure no arguments")
  //{
  //Procedure p = env.get_proc(Atom("-"));
  //REQUIRE_THROWS_AS(p(args), SemanticError);
  //}
  INFO("subneg procedure doble complex")

  {
    Procedure p = env.get_proc(Atom("-"));
    args.emplace_back(1.0);
    args.emplace_back(std::complex<double>(3.0, 8.0));
    REQUIRE(p(args) == Expression(std::complex<double>(-2.0, -8.0)));
  }
}

TEST_CASE("Test subneg complex double", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("subneg procedure doble complex")
  {
    Procedure p = env.get_proc(Atom("-"));
    args.emplace_back(std::complex<double>(3.0, 8.0));
    args.emplace_back(1.0);
    REQUIRE(p(args) == Expression(std::complex<double>(2.0, 8.0)));
  }
}
TEST_CASE("Test subneg complex", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  INFO("subneg procedure negitive complex")
  {
    Procedure p = env.get_proc(Atom("-"));
    args.emplace_back(std::complex<double>(3.0, 8.0));
    REQUIRE(p(args) == Expression(std::complex<double>(-3.0, -8.0)));
  }
  INFO("subneg procedure complex - complex")
  {
    Procedure p = env.get_proc(Atom("-"));
    args.emplace_back(std::complex<double>(1.0, 2.0));
    REQUIRE(p(args) == Expression(std::complex<double>(2.0, 6.0)));
  }
}
TEST_CASE("Test subneg double", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("-"));
    args.emplace_back(-7.0);
    REQUIRE(p(args) == Expression(7.0));
  }
}
TEST_CASE("Test mul double zero", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  //INFO ("mul procedure no arguments")
  //{
  //Procedure p = env.get_proc(Atom("*"));
  //REQUIRE_THROWS_AS(p(args), SemanticError);
  //}

  INFO("mul procedure")
  {
    Procedure p = env.get_proc(Atom("*"));
    args.emplace_back(1.0);
    args.emplace_back(0.0);
    REQUIRE(p(args) == Expression(0.0));
  }
}
TEST_CASE("Test mul double Idenity", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("*"));
    args.emplace_back(1.0);
    args.emplace_back(7.0);
    REQUIRE(p(args) == Expression(7.0));
  }
}

TEST_CASE("Test mul double and Complex", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  INFO("mul procedure");
  {
    Procedure p = env.get_proc(Atom("*"));
    args.emplace_back(std::complex<double>(1.0, 2.0));
    args.emplace_back(8.0);
    REQUIRE(p(args) == Expression(std::complex<double>(8.0, 16.0)));
  }

  INFO("mul procedure multiply aditional complex number");
  {
    Procedure p = env.get_proc(Atom("*"));
    args.emplace_back(std::complex<double>(1.0, 2.0));
    REQUIRE(p(args) == Expression(std::complex<double>(-24.0, 32.0)));
  }
}
TEST_CASE("Test mul double negaitive", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("*"));
    args.emplace_back(-2.0);
    args.emplace_back(3.0);
    REQUIRE(p(args) == Expression(-6.0));
  }
}
TEST_CASE("Test div double zero", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("div procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("/"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("div procedure");
  {
    Procedure p = env.get_proc(Atom("/"));
    args.emplace_back(10.0);
    args.emplace_back(5.0);
    REQUIRE(p(args) == Expression(2.0));
  }
}
TEST_CASE("Test div double Idenity", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("/"));
    args.emplace_back(7.0);
    args.emplace_back(1.0);
    REQUIRE(p(args) == Expression(7.0));
  }
}

TEST_CASE("Test div complex-complex", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("/"));
    args.emplace_back(std::complex<double>(6.0, 4.0));
    args.emplace_back(std::complex<double>(2.0, 2.0));
    REQUIRE(p(args) == Expression(std::complex<double>(2.5, -0.5)));
  }
}
TEST_CASE("Test div double-complex negitive", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("/"));
    args.emplace_back(-2.0);
    args.emplace_back(std::complex<double>(0.5, 1));
    REQUIRE(p(args) == Expression(std::complex<double>(-0.8, 1.6)));
  }
}
TEST_CASE("Test div complex-double negitive", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("/"));
    args.emplace_back(std::complex<double>(4.0, 1.0));
    args.emplace_back(-2.0);
    REQUIRE(p(args) == Expression(std::complex<double>(-2.0, -0.5)));
  }
}

TEST_CASE("Test sqrt double", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("sqrt procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("sqrt"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("sqrt procedure");
  {
    Procedure p = env.get_proc(Atom("sqrt"));
    args.emplace_back(4.0);
    REQUIRE(p(args) == Expression(2.0));
  }
}
TEST_CASE("Test sqrt double negitive", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("sqrt"));
    args.emplace_back(-4.0);
    REQUIRE(p(args) == Expression(std::complex<double>(0.0, 2.0)));
  }
}
TEST_CASE("Test sqrt complex ", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  {
    Procedure p = env.get_proc(Atom("sqrt"));
    args.emplace_back(std::complex<double>(0.0, 2.0));
    REQUIRE(p(args) == Expression(std::complex<double>(1.0, 1.0)));
  }
}
TEST_CASE("Test exp double double", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  INFO("exp procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("^"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("exp procedure");
  {
    Procedure p = env.get_proc(Atom("^"));
    args.emplace_back(4.0);
    args.emplace_back(2.0);
    REQUIRE(p(args) == Expression(16.0));
  }
}
TEST_CASE("Test exp complex complex", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("exp procedure complex ");
  {
    Procedure p = env.get_proc(Atom("^"));
    args.emplace_back(std::complex<double>(4.0, 1.0));
    args.emplace_back(std::complex<double>(4.0, 1.0));
    REQUIRE(p(args) == Expression(std::pow(std::complex<double>(4.0, 1.0), std::complex<double>(4.0, 1.0))));
  }
}
TEST_CASE("Test exp complex double", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("exp procedure complex ");
  {
    Procedure p = env.get_proc(Atom("^"));
    args.emplace_back(std::complex<double>(4.0, 1.0));
    args.emplace_back(4.0);
    REQUIRE(p(args) == Expression(std::pow(std::complex<double>(4.0, 1.0), 4.0)));
  }
}
TEST_CASE("Test exp double-complex", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("exp procedure complex ");
  {
    Procedure p = env.get_proc(Atom("^"));
    args.emplace_back(4.0);
    args.emplace_back(std::complex<double>(4.0, 1.0));
    REQUIRE(p(args) == Expression(std::pow(4.0, std::complex<double>(4.0, 1.0))));
  }
}
TEST_CASE("Test ln", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("ln procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("ln"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("ln procedure");
  {
    Procedure p = env.get_proc(Atom("ln"));
    args.emplace_back(4.0);
    REQUIRE(p(args) == Expression(std::log(4.0)));
  }
}
TEST_CASE("Test sin", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("sin procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("sin"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("sin procedure");
  {
    Procedure p = env.get_proc(Atom("sin"));
    args.emplace_back(4.0);
    REQUIRE(p(args) == Expression(std::sin(4.0)));
  }
}

TEST_CASE("Test cos", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("cos procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("cos"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("cos procedure");
  {
    Procedure p = env.get_proc(Atom("cos"));
    args.emplace_back(4.0);
    REQUIRE(p(args) == Expression(std::cos(4.0)));
  }
}
TEST_CASE("Test tan", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("tan procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("tan"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }
  INFO("tan procedure");
  {
    Procedure p = env.get_proc(Atom("tan"));
    args.emplace_back(4.0);
    REQUIRE(p(args) == Expression(std::tan(4.0)));
  }
}

TEST_CASE("Test real", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("real procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("real"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("real procedure");
  {
    Procedure p = env.get_proc(Atom("real"));
    args.emplace_back(std::complex<double>(4.0, 2.0));
    REQUIRE(p(args) == Expression(4.0));
  }
}
TEST_CASE("Test imag", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("imag procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("imag"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("imag procedure");
  {
    Procedure p = env.get_proc(Atom("imag"));
    args.emplace_back(std::complex<double>(4.0, 2.0));
    REQUIRE(p(args) == Expression(2.0));
  }
}
TEST_CASE("Test mag", "[environment]")
{
  Environment env;
  std::vector<Expression> args;
  INFO("mag procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("mag"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("mag procedure");
  {
    Procedure p = env.get_proc(Atom("mag"));
    args.emplace_back(std::complex<double>(0.0, 2.0));
    REQUIRE(p(args) == Expression(2.0));
  }
}
TEST_CASE("Test arg", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("arg procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("arg"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("arg procedure");
  {
    Procedure p = env.get_proc(Atom("arg"));
    args.emplace_back(std::complex<double>(4.0, 2.0));
    REQUIRE(p(args) == Expression(arg(std::complex<double>(4.0, 2.0))));
  }
}
TEST_CASE("Test conj", "[environment]")
{
  Environment env;
  std::vector<Expression> args;

  INFO("conj procedure no arguments")
  {
    Procedure p = env.get_proc(Atom("conj"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }

  INFO("conj procedure");
  {
    Procedure p = env.get_proc(Atom("conj"));
    args.emplace_back(std::complex<double>(4.0, 2.0));
    REQUIRE(p(args) == Expression(conj(std::complex<double>(4.0, 2.0))));
  }
}

TEST_CASE("Test Empty List", "[environment]")
{
	Environment env;

	std::vector<Expression> args;
	INFO("List procedure with empty List")
	{
		Procedure p = env.get_proc(Atom("list"));
		//args.emplace_back("list");
		CHECK(p(args) == Expression(std::vector<Expression>()));
	}

	INFO("First Procedure with empthy list")
	{
		Procedure p = env.get_proc(Atom("first"));
		args.emplace_back(std::vector<Expression>());
		REQUIRE_THROWS_AS(p(args), SemanticError);
	}
	INFO("Rest procedure empty list")
	{
		Procedure p = env.get_proc(Atom("rest"));
		REQUIRE_THROWS_AS(p(args), SemanticError);
	}
	INFO("Length procedure empty list")
	{
		Procedure p = env.get_proc(Atom("length"));
		REQUIRE(p(args) == Expression(0));
	}
}
TEST_CASE("Test List One arg", "[environment]")
{
	Environment env;

	std::vector<Expression> args;
    std::vector<Expression> test;
	test.emplace_back(Atom("list"));
	args.emplace_back(Atom("list"));
	args.emplace_back(4);
	test.emplace_back(4);
	INFO("List procedure with one list item")
	{
		Procedure p = env.get_proc(Atom("list"));		
        CHECK(p(args) == test);
		
	}
	INFO("First procedure with one list item")
	{
		Procedure p = env.get_proc(Atom("first"));
        //args.emplace_back(4);
		CHECK(p(args) == Expression(4));
	}
	INFO("rest procedure with one list item")
	{
		Procedure p = env.get_proc(Atom("rest"));
		CHECK_THROWS_AS(p(args), SemanticError);
	}
	INFO("length procedure with one list item")
	{
		Procedure p = env.get_proc(Atom("length"));
		CHECK(p(args) == Expression(1));
	}
	
}

TEST_CASE("Test reset", "[environment]")
{
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

TEST_CASE("Test semeantic errors", "[environment]")
{

  Environment env;
  std::vector<Expression> args;

  INFO("Begin semantic error");
  {
    Expression exp(Atom("begin"));

    REQUIRE_THROWS_AS(exp.eval(env), SemanticError);
  }
  // Test procedures that take one input for an invalit type
  INFO("Test add procedure semantic error: not a Number");
  {
    Procedure p = env.get_proc(Atom("+"));
    args.emplace_back(Atom("f"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }
  INFO("Test Mul procedure semantic error: Not a Number");
  {
    Procedure p = env.get_proc(Atom("*"));
    //args.emplace_back(Atom("f"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
  }
  INFO("Test Sub procedure semantic error: Not a Number");
  {
    Procedure p = env.get_proc(Atom("-"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
    //args.emplace_back(Atom("f"));
  }
  INFO("Test sqrt procedure semantic error: Not a Number");
  {
    Procedure p = env.get_proc(Atom("sqrt"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
    //args.emplace_back(Atom("f"));
  }
  INFO("Test ln procedure semantic error: Not a Number");
  {
    Procedure p = env.get_proc(Atom("ln"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
    //args.emplace_back(Atom("f"));
  }
  INFO("Test sin procedure semantic error: Not a Number");
  {
    Procedure p = env.get_proc(Atom("sin"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
    //args.emplace_back(Atom("f"));
  }
  INFO("Test cos procedure semantic error: Not a Number");
  {
    Procedure p = env.get_proc(Atom("cos"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
    //args.emplace_back(Atom("f"));
  }
  INFO("Test tan procedure semantic error: Not a Number");
  {
    Procedure p = env.get_proc(Atom("tan"));
    REQUIRE_THROWS_AS(p(args), SemanticError);
    //args.emplace_back(Atom("f"));
  }
}
