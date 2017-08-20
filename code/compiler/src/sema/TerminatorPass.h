#ifndef TERMINATOR_PASS_H
#define TERMINATOR_PASS_H

#include "Pass.h"
#include <list>


class TerminatorPass : public Pass
{
public:
  TerminatorPass() {}
  ~TerminatorPass() {}

  bool visitBlockStmt(BlockStmt* b) override;

  bool visitReturnStmt(ReturnStmt* s) override;
  bool visitBreakStmt(BreakStmt* s) override;
  bool visitContinueStmt(ContinueStmt* s) override;

private:
  std::list<BlockStmt*> blocks;
};

#endif /* TERMINATOR_PASS_H */
