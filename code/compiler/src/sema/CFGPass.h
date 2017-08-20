#ifndef CFG_PASS_H
#define CFG_PASS_H

#include "Pass.h"


class CFGPass : public Pass
{
public:
  CFGPass();
  ~CFGPass();

  bool visitForStmt(ForStmt*) override;
  void visitAfterForStmt(ForStmt*) override;

  bool visitWhileStmt(WhileStmt*) override;
  void visitAfterWhileStmt(WhileStmt*) override;

  bool visitUnlessStmt(UnlessStmt*) override;
  void visitAfterUnlessStmt(UnlessStmt*) override;

  bool visitLoopStmt(LoopStmt*) override;
  void visitAfterLoopStmt(LoopStmt*) override;

  bool visitBreakStmt(BreakStmt*) override;

  bool visitContinueStmt(ContinueStmt*) override;

private:
  size_t loops;
};

#endif /* CFG_PASS_H */
