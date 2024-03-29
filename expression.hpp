/*! \file expression.hpp
Defines the Expression type and assiciated functions.
 */
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <vector>

#include "atom.hpp"
#include "token.hpp"
#include <list>
#include <map>
#include <utility>


// forward declare Environment
class Environment;

/*! \class Expression
\brief An expression is a tree of Atoms.
An expression is an atom called the head followed by a (possibly empty)
list of expressions called the tail.
 */
class Expression
{
public:
  typedef std::vector<Expression>::const_iterator ConstIteratorType;

  /// Default construct and Expression, whose type in NoneType
  Expression();

  /*! Construct an Expression with given Atom as head an empty tail
    \param atom the atom to make the head
  */
  Expression(const Atom& a);

  /// deep-copy construct an expression (recursive)
  Expression(const Expression& a);

  /// Construct an expression with Property-list  
  Expression(const Expression& a,std::map<std::string, Expression>& es);
 
  /// Construct an expression of a list type;
  Expression(const std::vector<Expression>& es);
 
  /// deep-copy assign an expression  (recursive)
  Expression& operator=(const Expression& a);

  /// return a reference to the head Atom
  Atom& head();

  /// return a const-reference to the head Atom
  const Atom& head() const;

  /// append Atom to tail of the expression
  void append(const Atom& a);

  /// return a pointer to the last expression in the tail, or nullptr
  Expression* tail();

  /// return a const-iterator to the beginning of tail
  ConstIteratorType tailConstBegin() const noexcept;

  /// return a const-iterator to the tail end
  ConstIteratorType tailConstEnd() const noexcept;

  /// returns a property entery
  Expression getPropertyList(std::string key);
   
  /// returns a property entery
  Expression setPropertyList(const Expression expression_to_add, std::map<std::string, Expression> property) const noexcept;

  /// returns  a DiscretePlot
  Expression setDiscretePlot(Expression DATA, Expression options);

  /// returns a continous plot
  Expression PlotBoardersAndOptions(double xmin, double xmax, double ymin, double ymax, double scaleX, double scaleY, Expression Discrete, Expression options);

  /// convienience member to determine if head atom is a number
  bool isHeadNumber() const noexcept;

  /// convienience member to determine if head atom is a number
  bool isHeadComplex() const noexcept;

  /// convienience member to determine if head atom is a symbol
  bool isHeadSymbol() const noexcept;

  /// convienience member to determine if head atom is a string
  bool isHeadString() const noexcept;
  
  /// convience method to determine if head atom is a list
  bool isHeadList() const noexcept;
    
  /// convience method to determine if head atom is a list
  bool isHeadLambda() const noexcept;  

  /// Convience method to determine if properties are empty
  bool isPropertyListEmpty() const noexcept;

  /// Evaluate expression using a post-order traversal (recursive)
  Expression eval(Environment& env);

  /// equality comparison for two expressions (recursive)
  bool operator==(const Expression& exp) const noexcept;
  
  
	Expression MakeRange(Expression);

private:
  // the head of the expression
  Atom m_head;
  std::map<std::string, Expression> m_propertyList;
  // the properties of an expression
  

  // the tail list is expressed as a vector for access efficiency
  // and cache coherence, at the cost of wasted memory.
  std::vector<Expression> m_tail;

  // convenience typedef
  typedef std::vector<Expression>::iterator IteratorType;

  // internal helper methods
  Expression handle_lookup(const Atom& head, const Environment& env);
  Expression handle_define(Environment& env);
  Expression handle_begin(Environment& env);
  Expression handle_lambda(Environment& env);
  Expression handle_apply(Environment& env);
  Expression handle_map(Environment& env);
  Expression store_lamba(Environment& env, Expression& original);
  Expression setContinuousPlot(Environment&env);
 
};

/// Render expression to output stream
std::ostream&
operator<<(std::ostream& out, const Expression& exp);

/// inequality comparison for two expressions (recursive)
bool
operator!=(const Expression& left, const Expression& right) noexcept;

#endif
