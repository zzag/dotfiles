#include "CodegenContext.h"


CodegenContext::CodegenContext()
{
  module = new llvm::Module("main", llvm::getGlobalContext());
  builder = new llvm::IRBuilder<>(llvm::getGlobalContext());
  passManager = new llvm::legacy::FunctionPassManager(module);
  declCodegen = new DeclCodegen(this);
  stmtCodegen = new StmtCodegen(this);
  exprCodegen = new ExprCodegen(this);

  passManager->add(llvm::createInstructionCombiningPass());
  passManager->add(llvm::createReassociatePass());
  passManager->add(llvm::createCFGSimplificationPass());
  passManager->doInitialization();
}


CodegenContext::~CodegenContext()
{
}


void
CodegenContext::generate(TopLevelDecl* root)
{
  root->accept(declCodegen);
}
