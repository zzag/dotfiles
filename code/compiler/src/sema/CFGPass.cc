#include "CFGPass.h"


CFGPass::CFGPass() : loops(0)
{
}


CFGPass::~CFGPass()
{
}


bool
CFGPass::visitForStmt(ForStmt*)
{
  loops++;
  return true;
}


void
CFGPass::visitAfterForStmt(ForStmt*)
{
  loops--;
}


bool
CFGPass::visitWhileStmt(WhileStmt*)
{
  loops++;
  return true;
}


void
CFGPass::visitAfterWhileStmt(WhileStmt*)
{
  loops--;
}


bool
CFGPass::visitUnlessStmt(UnlessStmt*)
{
  loops++;
  return true;
}


void
CFGPass::visitAfterUnlessStmt(UnlessStmt*)
{
  loops--;
}


bool
CFGPass::visitLoopStmt(LoopStmt*)
{
  loops++;
  return true;
}


void
CFGPass::visitAfterLoopStmt(LoopStmt*)
{
  loops--;
}


bool
CFGPass::visitBreakStmt(BreakStmt*)
{
  if (! loops)
    error("break statement is used outside of loop statement");
  return false;
}


bool
CFGPass::visitContinueStmt(ContinueStmt*)
{
  if (! loops)
    error("continue statement is used outside of loop statement");
  return false;
}
