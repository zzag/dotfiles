#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser/AST.h"
#include "llvm/IR/Type.h"


class CodegenContext;
class Codegen : public Visitor
{
public:
  Codegen(CodegenContext* context) : context(context)
  {
  }

  virtual ~Codegen() {}

protected:
  llvm::Type* getType(std::string type);

protected:
  CodegenContext* context;
};

#endif /* CODEGEN_H */
