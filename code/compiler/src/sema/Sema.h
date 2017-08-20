#ifndef SEMA_H
#define SEMA_H

#include "Pass.h"
#include <vector>


class Sema
{
public:
  Sema() {}
  ~Sema() {}

  void add(Pass* pass) { passes.push_back(pass); }

  bool isValid(TopLevelDecl* root)
  {
    for (auto pass : passes)
    {
      pass->reset();
      root->accept(pass);
      if (pass->anyErrors()) return false;
    }

    return true;
  }

private:
  std::vector<Pass*> passes;
};

#endif /* SEMA_H */
