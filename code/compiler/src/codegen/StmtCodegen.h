#ifndef STMT_CODEGEN_H
#define STMT_CODEGEN_H

#include "Codegen.h"
#include "llvm/IR/BasicBlock.h"
#include <list>


struct Loop
{
  Loop(llvm::BasicBlock* entry, llvm::BasicBlock* exit) :
      entry(entry), exit(exit)
  {
  }

  llvm::BasicBlock* entry;
  llvm::BasicBlock* exit;
};


class StmtCodegen : public Codegen
{
public:
  StmtCodegen(CodegenContext* context) : Codegen(context)
  {
  }
  ~StmtCodegen() {}

  bool visitBlockStmt(BlockStmt* s) override;

  bool visitVarDecl(VarDecl* d) override;
  bool visitExprStmt(ExprStmt* e) override;

  bool visitIfStmt(IfStmt* s) override;
  bool visitForStmt(ForStmt* s) override;
  bool visitWhileStmt(WhileStmt* s) override;
  bool visitUnlessStmt(UnlessStmt* s) override;
  bool visitLoopStmt(LoopStmt* s) override;
  bool visitBreakStmt(BreakStmt* s) override;
  bool visitContinueStmt(ContinueStmt* s) override;

  bool visitReturnStmt(ReturnStmt* r) override;

private:
  Loop& currentLoop() { return loops.front(); }
  void pushLoop(Loop& loop) { loops.push_front(loop); }
  void popLoop() { loops.pop_front(); }

private:
  std::list<Loop> loops;
};

#endif /* STMT_CODEGEN_H */
