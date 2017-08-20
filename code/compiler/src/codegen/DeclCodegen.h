#ifndef DECL_CODEGEN_H
#define DECL_CODEGEN_H

#include "Codegen.h"


class DeclCodegen : public Codegen
{
public:
  DeclCodegen(CodegenContext* context) : Codegen(context)
  {
  }
  ~DeclCodegen() {}

  bool visitTopLevelDecl(TopLevelDecl* d) override;
  void visitAfterTopLevelDecl(TopLevelDecl* d) override;

  bool visitExternFuncDecl(ExternFuncDecl* f) override;
  bool visitFuncDecl(FuncDecl* f) override;
};

#endif /* DECL_CODEGEN_H */
