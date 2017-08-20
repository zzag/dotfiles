#include "DeclCodegen.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "CodegenContext.h"
#include <vector>


using namespace llvm;


bool
DeclCodegen::visitTopLevelDecl(TopLevelDecl* d)
{
  context->getFuncs().pushBlock();
  return true;
}


void
DeclCodegen::visitAfterTopLevelDecl(TopLevelDecl* d)
{
  context->getFuncs().popBlock();
}


bool
DeclCodegen::visitExternFuncDecl(ExternFuncDecl* f)
{
  ProtoDecl* proto = f->getProto();
  std::vector<Type*> args;
  for (auto arg : proto->getArgs())
    args.push_back(getType(((VarDecl*)arg)->getType()));
  FunctionType* funcType = FunctionType::get(getType(proto->getReturnType()), args, false);
  Function* func = Function::Create(funcType, 
      Function::ExternalLinkage, proto->getId(), context->getModule());

  Function::arg_iterator ai = func->arg_begin();
  for (auto arg : proto->getArgs())
  {
    VarDecl* var = reinterpret_cast<VarDecl*>(arg);
    (*ai).setName(var->getId().c_str());
    ai++;
  }

  context->getFuncs().declare(proto->getId(), proto);
  return false;
}


bool
DeclCodegen::visitFuncDecl(FuncDecl* f)
{
  IRBuilder<>& builder = context->getBuilder();
  ProtoDecl* proto = f->getProto();
  std::vector<Type*> argTypes;
  for (auto arg : proto->getArgs())
    argTypes.push_back(getType(((VarDecl*)arg)->getType()));
  FunctionType* funcType = FunctionType::get(getType(proto->getReturnType()), argTypes, false);
  Function* func = Function::Create(funcType, 
      Function::ExternalLinkage, proto->getId(), context->getModule());

  BasicBlock* bb = BasicBlock::Create(getGlobalContext(), "entry", func);
  builder.SetInsertPoint(bb);
  context->getSymbolTable().pushBlock();
  context->getTypes().pushBlock();

  // generate arguments
  Function::arg_iterator ai = func->arg_begin();
  DeclList& args = proto->getArgs();
  for (unsigned i = 0, e = args.size(); i != e; i++, ai++)
  {
    VarDecl* var = reinterpret_cast<VarDecl*>(args.at(i));
    AllocaInst* alloca = builder.CreateAlloca(
      getType(var->getType()),
      0,
      var->getId().c_str()
    );
    builder.CreateStore(ai.getNodePtrUnchecked(), alloca);
    context->getSymbolTable().declare(var->getId(), alloca);
    context->getTypes().declare(var->getId(), var->getType());
  }

  // generate body
  BlockStmt* body = reinterpret_cast<BlockStmt*>(f->getBody());
  for (auto stmt : body->getStmts())
    stmt->accept(context->getStmtCodegen());

  context->getTypes().popBlock();
  context->getSymbolTable().popBlock();
  context->getFuncs().declare(proto->getId(), proto);
  
  BasicBlock* lastBB = builder.GetInsertBlock();
  if (! lastBB->getTerminator()) builder.CreateRetVoid();

  verifyFunction(*func, &outs());
  context->getPassManager()->run(*func);
  return false;
}
