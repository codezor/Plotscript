#include "atom.hpp"

#include <cctype>
#include <cmath>
#include <limits>
#include <sstream>

Atom::Atom()
  : m_type(NoneKind)
{}

Atom::Atom(double value)
{

  setNumber(value);
}

Atom::Atom(std::complex<double> value)
{

  setComplex(value);
}

Atom::Atom(const Token& token)
  : Atom()
{
  // is token a number?
  double temp;
  std::istringstream iss(token.asString());
  if (iss >> temp) {
    // check for trailing characters if >> succeeds
    if (iss.rdbuf()->in_avail() == 0) {
      setNumber(temp);
    }
  } else {
    // make sure does not start with number
    if (!std::isdigit(token.asString()[0])) {
      // Testing symbol for I type
      if (token.asString() == "I") {
        setComplex(std::complex<double>(0.0, 1.0));
      }
	  if (token.asString().size() >= 2 && token.asString().front() == '"' && token.asString().back() == '"') {
		  setString(token.asString());
	  }
	  else {
		  setSymbol(token.asString());
	  }
	  
    }
  }
}

Atom::Atom(const std::string& value)
  : Atom()
{
	if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
		setString(value);
	}
	else {
		setSymbol(value);
	}
}

Atom::Atom(const Atom& x)
  : Atom()
{
  if (x.isNumber()) {
    setNumber(x.m_numberValue);
  } else if (x.isComplex()) {
    setComplex(x.m_complexValue);
  } else if (x.isSymbol()) {
    setSymbol(x.m_stringValue);
  }
  else if(x.isString()) 
  {
	  setString(x.m_stringValue);
  }
}

Atom&
Atom::operator=(const Atom& x)
{

  if (this != &x) {
    if (x.m_type == NoneKind) {
      m_type = NoneKind;
    } else if (x.m_type == NumberKind) {
      setNumber(x.m_numberValue);
    } else if (x.m_type == ComplexKind) {
      setComplex(x.m_complexValue);
    } else if (x.m_type == SymbolKind) {
      setSymbol(x.m_stringValue);
    }
	else if (x.m_type == StringKind) {
		setString(x.m_stringValue);
	}
  }
  return *this;
}

Atom::~Atom()
{
  // we need to ensure the destructor of the symbol string is called
  if (m_type == SymbolKind || m_type == StringKind) {
    m_stringValue.~basic_string();
  }
}

bool
Atom::isNone() const noexcept
{
  return m_type == NoneKind;
}

bool
Atom::isNumber() const noexcept
{
  return m_type == NumberKind;
}

bool
Atom::isComplex() const noexcept
{
  return m_type == ComplexKind;
}

bool
Atom::isSymbol() const noexcept
{
  return m_type == SymbolKind;
}

bool
Atom::isString() const noexcept
{
	return m_type == StringKind;
}

void
Atom::setNumber(double value)
{

  m_type = NumberKind;
  m_numberValue = value;
}

void
Atom::setComplex(std::complex<double> value)
{

  m_type = ComplexKind;
  m_complexValue = value;
}

void
Atom::setSymbol(const std::string& value)
{
  // we need to ensure the destructor of the symbol string is called
  if (m_type == SymbolKind || m_type == StringKind) {
    m_stringValue.~basic_string();
  }

  m_type = SymbolKind;

  // copy construct in place
  new (&m_stringValue) std::string(value);
}

void Atom::setString(const std::string& value)
{
	setSymbol(value);
	m_type = StringKind;

}
double
Atom::asNumber() const noexcept
{

  return (m_type == NumberKind) ? m_numberValue : 0.0;
}

std::complex<double>
Atom::asComplex() const noexcept
{

  return ((m_type == ComplexKind) ? m_complexValue
                                  : std::complex<double>(0.0, 0.0));
}

std::string
Atom::asSymbol() const noexcept
{

  std::string result;

  if (m_type == SymbolKind) {
    result = m_stringValue;
  }

  return result;
}

std::string
Atom::asString() const noexcept
{

	std::string result;

	if (m_type == StringKind) {
		result = m_stringValue;
	}
	else if (m_type == SymbolKind && m_stringValue == "NONE")
	{
		result = m_stringValue;
	}

	return result;
}

bool
Atom::operator==(const Atom& right) const noexcept
{

  if (m_type != right.m_type)
    return false;

  switch (m_type) {
    case NoneKind:
      if (right.m_type != NoneKind)
        return false;
      break;
    case NumberKind: {
      if (right.m_type != NumberKind)
        return false;
      double dleft = m_numberValue;
      double dright = right.m_numberValue;
      double diff = fabs(dleft - dright);
      if (std::isnan(diff) || (diff > std::numeric_limits<double>::epsilon()))
        return false;
    } break;

    // TODO: check that this is correct
    case ComplexKind: {
      if (right.m_type != ComplexKind)
        return false;
      std::complex<double> dleft = m_complexValue;
      std::complex<double> dright = right.m_complexValue;
      double diff = fabs((abs(dleft) - abs(dright)));
      if (std::isnan(diff) || (diff > std::numeric_limits<double>::epsilon()))
        return false;
    } break;
    case SymbolKind: {
      if (right.m_type != SymbolKind)
        return false;

      return m_stringValue == right.m_stringValue;
    } break;
	case StringKind: {
		if (right.m_type != StringKind)
			return false;

		return m_stringValue == right.m_stringValue;
	} break;
    default:
      return false;
  }

  return true;
}

bool
operator!=(const Atom& left, const Atom& right) noexcept
{

  return !(left == right);
}

std::ostream&
operator<<(std::ostream& out, const Atom& a)
{

  if (a.isNumber()) {
    out << a.asNumber();
  }
  if (a.isComplex()) {
    std::complex<double> value = a.asComplex();
    out << std::real(value) << "," << std::imag(value);
  }
  if (a.isSymbol()) {
    out << a.asSymbol();
  }
  if (a.isString()) {
    out << a.asString();
  }

  return out;
}
