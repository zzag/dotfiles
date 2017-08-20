#include "TerminatorPass.h"


bool
TerminatorPass::visitBlockStmt(BlockStmt* b)
{
  blocks.push_front(b);
  for (auto stmt : b->getStmts())
    stmt->accept(this);
  blocks.pop_front();
  return false;
}


bool
TerminatorPass::visitReturnStmt(ReturnStmt* s)
{
  BlockStmt* block = blocks.front();
  Stmt* last = block->getStmts().back();
  if (s != last) error("dead code");
  return false;
}


bool
TerminatorPass::visitBreakStmt(BreakStmt* s)
{
  BlockStmt* block = blocks.front();
  Stmt* last = block->getStmts().back();
  if (s != last) error("dead code");
  return false;
}


bool
TerminatorPass::visitContinueStmt(ContinueStmt* s)
{
  BlockStmt* block = blocks.front();
  Stmt* last = block->getStmts().back();
  if (s != last) error("dead code");
  return false;
}
