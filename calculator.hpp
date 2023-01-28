#pragma once
#include <iostream>
#include <string>

class AbstractToken {
  public:
  AbstractToken(const std::string&);
  virtual const std::string& GetStringToken() const = 0;
};
// "10 + 22 - (748 - 2840 + 567890)" ->
// + 10 22 - (...) ->
// +10 - 22 (...) ->
// +10 - 22 
class BracketToken : public AbstractToken {
  public:
  BracketToken(const std::string&);
  bool isOpening() const;

  private:

};

template <typename T>
class OperandToken : public AbstractToken {
  OperandToken(const std:string&);
  OperandToken(const T& value);
  const T& GetValue();
};

class OperatorToken {
  OperatorToken(const std::string);
  bool isBinary();
  Calculate(Operand* lhs, Operand* rhs); //
  
};


class calculator
{
private:
  /* data */
public:
  calculator(/* args */);
  ~calculator();
};

calculator::calculator(/* args */)
{
}

calculator::~calculator()
{
}
