#ifndef PASS_H
#define PASS_H

#include "parser/AST.h"
#include <string>
#include <iostream>


class Pass : public Visitor
{
public:
  Pass() : totalErrors(0) {}
  ~Pass() {}

  bool anyErrors() { return totalErrors > 0; }
  void error(std::string message = "")
  {
    if (! message.empty()) std::cout << message << std::endl;
    totalErrors++;
  }
  size_t getTotalErrors() { return totalErrors; }
  void reset() { totalErrors = 0; }

private:
  size_t totalErrors;
};

#endif /* PASS_H */
