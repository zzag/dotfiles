#ifndef COMPILER_FRONTEND_H
#define COMPILER_FRONTEND_H

#include "parser/AST.h"
#include "llvm/IR/Module.h"
#include <string>


class CompilerFrontend
{
public:
  CompilerFrontend(std::string& filename);
  ~CompilerFrontend();

  TopLevelDecl* buildAST();
  llvm::Module* codegen();
  void emitIR();

private:
  std::string filename;
};

#endif /* COMPILER_FRONTEND_H */
