#include "catch.hpp"

#include "atom.hpp"

TEST_CASE("Test constructors", "[atom]")
{

  {
    INFO("Default Constructor");
    Atom a;

    REQUIRE(a.isNone());
    REQUIRE(!a.isNumber());
    REQUIRE(!a.isComplex());
    REQUIRE(!a.isSymbol());
  }

  {
    INFO("Number Constructor");
    Atom a(1.0);

    REQUIRE(!a.isNone());
    REQUIRE(a.isNumber());
    REQUIRE(!a.isComplex());
    REQUIRE(!a.isSymbol());
  }

  {
    INFO("Complex Constructor");
    Atom a(std::complex<double>(0.0, 1.0));

    REQUIRE(!a.isNone());
    REQUIRE(!a.isNumber());
    REQUIRE(a.isComplex());
    REQUIRE(!a.isSymbol());
  }

  {
    INFO("Symbol Constructor");
    Atom a("hi");

    REQUIRE(!a.isNone());
    REQUIRE(!a.isNumber());
    REQUIRE(!a.isComplex());
    REQUIRE(a.isSymbol());
  }

  {
    INFO("Token Constructor");
    Token t("hi");
    Atom a(t);

    REQUIRE(!a.isNone());
    REQUIRE(!a.isNumber());
    REQUIRE(!a.isComplex());
    REQUIRE(a.isSymbol());
  }

  {
    INFO("Copy Constructor");
    Atom a("hi");
    Atom b(1.0);
    Atom e(std::complex<double>(0.0, 1.0));

    Atom c = a;
    REQUIRE(!a.isNone());
    REQUIRE(!c.isNumber());
    REQUIRE(!c.isComplex());
    REQUIRE(c.isSymbol());

    Atom d = b;
    REQUIRE(!a.isNone());
    REQUIRE(d.isNumber());
    REQUIRE(!d.isComplex());
    REQUIRE(!d.isSymbol());

    Atom f = e;
    REQUIRE(!e.isNone());
    REQUIRE(!f.isNumber());
    REQUIRE(f.isComplex());
    REQUIRE(!f.isSymbol());
  }
}

TEST_CASE("Test assignment", "[atom]")
{

  {
    INFO("default to default");
    Atom a;
    Atom b;
    b = a;
    REQUIRE(b.isNone());
    REQUIRE(!b.isNumber());
    REQUIRE(!b.isComplex());
    REQUIRE(!b.isSymbol());
  }

  {
    INFO("default to number");
    Atom a;
    Atom b(1.0);
    b = a;
    REQUIRE(b.isNone());
    REQUIRE(!b.isNumber());
    REQUIRE(!b.isComplex());
    REQUIRE(!b.isSymbol());
  }

  {
    INFO("default to symbol");
    Atom a;
    Atom b("hi");
    b = a;
    REQUIRE(b.isNone());
    REQUIRE(!b.isNumber());
    REQUIRE(!b.isComplex());
    REQUIRE(!b.isSymbol());
  }

  {
    INFO("default to complex");
    Atom a;
    Atom b(std::complex<double>(1.0, 1.0));
    b = a;
    REQUIRE(b.isNone());
    REQUIRE(!b.isNumber());
    REQUIRE(!b.isComplex());
    REQUIRE(!b.isSymbol());
  }

  {
    INFO("number to default");
    Atom a(1.0);
    Atom b;
    b = a;
    REQUIRE(b.isNumber());
    REQUIRE(b.asNumber() == 1.0);
  }

  {
    INFO("number to number");
    Atom a(1.0);
    Atom b(2.0);
    b = a;
    REQUIRE(b.isNumber());
    REQUIRE(b.asNumber() == 1.0);
  }

  {
    INFO("number to symbol");
    Atom a("hi");
    Atom b(1.0);
    b = a;
    REQUIRE(b.isSymbol());
    REQUIRE(b.asSymbol() == "hi");
  }

  {
    INFO("number to complex");
    Atom a(std::complex<double>(1.0, 1.0));
    Atom b(1.0);
    b = a;
    REQUIRE(b.isComplex());
    REQUIRE(b.asComplex() == std::complex<double>(1.0, 1.0));
  }

  {
    INFO("symbol to default");
    Atom a("hi");
    Atom b;
    b = a;
    REQUIRE(b.isSymbol());
    REQUIRE(b.asSymbol() == "hi");
  }

  {
    INFO("symbol to number");
    Atom a("hi");
    Atom b(1.0);
    b = a;
    REQUIRE(b.isSymbol());
    REQUIRE(b.asSymbol() == "hi");
  }

  {
    INFO("symbol to symbol");
    Atom a("hi");
    Atom b("bye");
    b = a;
    REQUIRE(b.isSymbol());
    REQUIRE(b.asSymbol() == "hi");
  }

  {
    INFO("symbol to complex");
    Atom a("hi");
    Atom b(std::complex<double>(1.0, 1.0));
    b = a;
    REQUIRE(b.isSymbol());
    REQUIRE(b.asSymbol() == "hi");
  }

  {
    INFO("complex to default");
    Atom a(std::complex<double>(1.0, 1.0));
    Atom b;
    b = a;
    REQUIRE(b.isComplex());
    REQUIRE(b.asComplex() == std::complex<double>(1.0, 1.0));
  }

  {
    INFO("complex to number");
    Atom a(std::complex<double>(1.0, 1.0));
    Atom b(1.0);
    b = a;
    REQUIRE(b.isComplex());
    REQUIRE(b.asComplex() == std::complex<double>(1.0, 1.0));
  }

  {
    INFO("complex to symbol");
    Atom a(std::complex<double>(1.0, 1.0));
    Atom b("bye");
    b = a;
    REQUIRE(b.isComplex());
    REQUIRE(b.asComplex() == std::complex<double>(1.0, 1.0));
  }

  {
    INFO("complex to complex");

    Atom a(std::complex<double>(1.0, 1.0));
    Atom b(std::complex<double>(2.0, 2.0));
    b = a;
    REQUIRE(b.isComplex());
    REQUIRE(b.asComplex() == std::complex<double>(1.0, 1.0));
  }
}

TEST_CASE("test comparison", "[atom]")
{

  {
    INFO("compare default to default");
    Atom a;
    Atom b;
    REQUIRE(a == b);
  }

  {
    INFO("compare default to number");
    Atom a;
    Atom b(1.0);
    REQUIRE(a != b);
  }

  {
    INFO("compare default to symbol");
    Atom a;
    Atom b("hi");
    REQUIRE(a != b);
  }

  {
    INFO("compare default to complex");
    Atom a;
    Atom b(std::complex<double>(0.0, 1.0));
    REQUIRE(a != b);
  }

  {
    INFO("compare number to default");
    Atom a(1.0);
    Atom b;
    REQUIRE(a != b);
  }

  {
    INFO("compare number to number");
    Atom a(1.0);
    Atom b(1.0);
    Atom c(2.0);
    REQUIRE(a == b);
    REQUIRE(a != c);
  }

  {
    INFO("compare number to symbol");
    Atom a(1.0);
    Atom b("hi");
    REQUIRE(a != b);
  }

  {
    INFO("compare number to symbol");
    Atom a(1.0);
    Atom b(std::complex<double>(0.0, 1.0));
    REQUIRE(a != b);
  }

  {
    INFO("compare symbol to default");
    Atom a("hi");
    Atom b;
    REQUIRE(a != b);
  }

  {
    INFO("compare symbol to number");
    Atom a("hi");
    Atom b(1.0);
    REQUIRE(a != b);
  }

  {
    INFO("compare symbol to symbol");
    Atom a("hi");
    Atom b("hi");
    Atom c("bye");
    REQUIRE(a == b);
    REQUIRE(a != c);
  }

  {
    INFO("compare symbol to complex");
    Atom a("hi");
    Atom b(std::complex<double>(0.0, 1.0));
    REQUIRE(a != b);
  }

  {
    INFO("compare complex to default");
    Atom a(std::complex<double>(0.0, 1.0));
    Atom b;
    REQUIRE(a != b);
  }

  {
    INFO("compare complex to number");
    Atom a(std::complex<double>(0.0, 1.0));
    Atom b(1.0);
    REQUIRE(a != b);
  }

  {
    INFO("compare complex to symbol");
    Atom a(std::complex<double>(0.0, 1.0));
    Atom b("hi");
    REQUIRE(a != b);
  }

  {
    INFO("compare complex to complex");
    Atom a(std::complex<double>(0.0, 1.0));
    Atom b(std::complex<double>(0.0, 1.0));
    Atom c(std::complex<double>(2.0, 2.0));
    REQUIRE(a == b);
    REQUIRE(a != c);
  }
}
