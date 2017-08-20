#include "StmtCodegen.h"
#include "CodegenContext.h"


using namespace llvm;


inline void
maybeCreateBr(IRBuilder<>& builder, BasicBlock* to)
{
  BasicBlock* current = builder.GetInsertBlock();
  if (! current->getTerminator()) builder.CreateBr(to);
}


bool
StmtCodegen::visitBlockStmt(BlockStmt* s)
{
  SymbolTable<std::string, AllocaInst*>& vars = context->getSymbolTable();
  SymbolTable<std::string, std::string>& types = context->getTypes();

  vars.pushBlock();
  types.pushBlock();
  for (auto stmt : s->getStmts())
    stmt->accept(this);
  types.popBlock();
  vars.popBlock();
  return false;
}


bool
StmtCodegen::visitVarDecl(VarDecl* v)
{
  Function* func = context->getBuilder().GetInsertBlock()->getParent();
  IRBuilder<> builder(&func->getEntryBlock(),
      func->getEntryBlock().begin());

  v->getInitialValue()->accept(context->getExprCodegen());
  Value* initialValue = context->getExprCodegen()->getValue();

  AllocaInst* alloca = builder.CreateAlloca(getType(v->getType()), 0, v->getId().c_str());
  context->getBuilder().CreateStore(initialValue, alloca);

  context->getSymbolTable().declare(v->getId(), alloca);
  context->getTypes().declare(v->getId(), v->getType());
  return false;
}


bool
StmtCodegen::visitExprStmt(ExprStmt* e)
{
  e->getExpr()->accept(context->getExprCodegen());
  return false;
}


bool
StmtCodegen::visitIfStmt(IfStmt* s)
{
  ExprCodegen* exprCodegen = context->getExprCodegen();
  IRBuilder<>& builder = context->getBuilder();

  Function* func = builder.GetInsertBlock()->getParent();
  BasicBlock* thenBB = BasicBlock::Create(getGlobalContext(), "if.then", func);
  BasicBlock* elseBB = BasicBlock::Create(getGlobalContext(), "if.else");
  BasicBlock* mergeBB = BasicBlock::Create(getGlobalContext(), "if.merge");

  s->getCond()->accept(exprCodegen);
  Value* condV = exprCodegen->getValue();
  builder.CreateCondBr(condV, thenBB, elseBB);

  builder.SetInsertPoint(thenBB);
  s->getThen()->accept(this);
  maybeCreateBr(builder, mergeBB);

  func->getBasicBlockList().push_back(elseBB);
  builder.SetInsertPoint(elseBB);
  if (s->getOtherwise()) s->getOtherwise()->accept(this);
  maybeCreateBr(builder, mergeBB);

  func->getBasicBlockList().push_back(mergeBB);
  builder.SetInsertPoint(mergeBB);
  return false;
}


bool
StmtCodegen::visitForStmt(ForStmt* s)
{
  ExprCodegen* exprCodegen = context->getExprCodegen();
  IRBuilder<>& builder = context->getBuilder();

  SymbolTable<std::string, AllocaInst*>& vars = context->getSymbolTable();
  SymbolTable<std::string, std::string>& types = context->getTypes();
  Function* func = builder.GetInsertBlock()->getParent();

  vars.pushBlock();
  types.pushBlock();

  BasicBlock* initBB = BasicBlock::Create(getGlobalContext(), "for.init");
  BasicBlock* condBB = BasicBlock::Create(getGlobalContext(), "for.cond");
  BasicBlock* stepBB = BasicBlock::Create(getGlobalContext(), "for.step");
  BasicBlock* bodyBB = BasicBlock::Create(getGlobalContext(), "for.body");
  BasicBlock* exitBB = BasicBlock::Create(getGlobalContext(), "for.exit");

  Loop loopItem(stepBB, exitBB);
  pushLoop(loopItem);

  builder.CreateBr(initBB);

  // generate init
  func->getBasicBlockList().push_back(initBB);
  builder.SetInsertPoint(initBB);
  s->getInit()->accept(this);
  builder.CreateBr(condBB);

  // generate cond
  func->getBasicBlockList().push_back(condBB);
  builder.SetInsertPoint(condBB);
  s->getCond()->accept(exprCodegen);
  Value* condV = exprCodegen->getValue();
  builder.CreateCondBr(condV, bodyBB, exitBB);

  // generate step
  func->getBasicBlockList().push_back(stepBB);
  builder.SetInsertPoint(stepBB);
  s->getStep()->accept(exprCodegen);
  builder.CreateBr(condBB);

  // generate body
  func->getBasicBlockList().push_back(bodyBB);
  builder.SetInsertPoint(bodyBB);
  s->getBody()->accept(this);
  maybeCreateBr(builder, stepBB);

  // after
  func->getBasicBlockList().push_back(exitBB);
  builder.SetInsertPoint(exitBB);

  popLoop();
  types.popBlock();
  vars.popBlock();
  return false;
}


bool
StmtCodegen::visitWhileStmt(WhileStmt* s)
{
  ExprCodegen* exprCodegen = context->getExprCodegen();
  IRBuilder<>& builder = context->getBuilder();

  Function* func = builder.GetInsertBlock()->getParent();
  BasicBlock* condBB = BasicBlock::Create(getGlobalContext(), "while.cond", func);
  BasicBlock* bodyBB = BasicBlock::Create(getGlobalContext(), "while.body");
  BasicBlock* exitBB = BasicBlock::Create(getGlobalContext(), "while.exit");

  Loop loopItem(condBB, exitBB);
  pushLoop(loopItem);
  builder.CreateBr(condBB);

  builder.SetInsertPoint(condBB);
  s->getCond()->accept(exprCodegen);
  Value* condV = exprCodegen->getValue();
  builder.CreateCondBr(condV, bodyBB, exitBB);

  func->getBasicBlockList().push_back(bodyBB);
  builder.SetInsertPoint(bodyBB);
  s->getBody()->accept(this);
  builder.CreateBr(condBB);

  func->getBasicBlockList().push_back(exitBB);
  builder.SetInsertPoint(exitBB);
  popLoop();

  return false;
}


bool
StmtCodegen::visitUnlessStmt(UnlessStmt* s)
{
  ExprCodegen* exprCodegen = context->getExprCodegen();
  IRBuilder<>& builder = context->getBuilder();

  Function* func = builder.GetInsertBlock()->getParent();
  BasicBlock* condBB = BasicBlock::Create(getGlobalContext(), "unless.cond", func);
  BasicBlock* bodyBB = BasicBlock::Create(getGlobalContext(), "unless.body");
  BasicBlock* exitBB = BasicBlock::Create(getGlobalContext(), "unless.exit");

  Loop loopItem(condBB, exitBB);
  pushLoop(loopItem);
  builder.CreateBr(condBB);

  builder.SetInsertPoint(condBB);
  s->getCond()->accept(exprCodegen);
  Value* condV = exprCodegen->getValue();
  builder.CreateCondBr(condV, exitBB, bodyBB);

  func->getBasicBlockList().push_back(bodyBB);
  builder.SetInsertPoint(bodyBB);
  s->getBody()->accept(this);
  builder.CreateBr(condBB);

  func->getBasicBlockList().push_back(exitBB);
  builder.SetInsertPoint(exitBB);
  popLoop();

  return false;
}


bool
StmtCodegen::visitLoopStmt(LoopStmt* s)
{
  IRBuilder<>& builder = context->getBuilder();

  Function* func = builder.GetInsertBlock()->getParent();
  BasicBlock* bodyBB = BasicBlock::Create(getGlobalContext(), "loop.body", func);
  BasicBlock* exitBB = BasicBlock::Create(getGlobalContext(), "loop.exit");

  Loop loopItem(bodyBB, exitBB);
  pushLoop(loopItem);
  builder.CreateBr(bodyBB);

  builder.SetInsertPoint(bodyBB);
  s->getBody()->accept(this);
  builder.CreateBr(bodyBB);

  func->getBasicBlockList().push_back(exitBB);
  builder.SetInsertPoint(exitBB);
  popLoop();

  return false;
}


bool
StmtCodegen::visitBreakStmt(BreakStmt* s)
{
  Loop& loop = currentLoop();
  IRBuilder<>& builder = context->getBuilder();

  maybeCreateBr(builder, loop.exit);
  return false;
}


bool
StmtCodegen::visitContinueStmt(ContinueStmt* s)
{
  Loop& loop = currentLoop();
  IRBuilder<>& builder = context->getBuilder();

  maybeCreateBr(builder, loop.entry);
  return false;
}


bool
StmtCodegen::visitReturnStmt(ReturnStmt* r)
{
  IRBuilder<>& builder = context->getBuilder();
  Expr* retValue = r->getValue();

  if (retValue)
  {
    retValue->accept(context->getExprCodegen());
    builder.CreateRet(context->getExprCodegen()->getValue());
  }
  else
  {
    builder.CreateRetVoid();
  }

  return false;
}
