#include "AST.h"

void 
TopLevelDecl::accept(Visitor *v)
{
  v->visitBeforeTopLevelDecl(this);
  if (v->visitTopLevelDecl(this))
  {
    for(auto decl : decls)
      decl->accept(v);
  }
  v->visitAfterTopLevelDecl(this);
}


void 
VarDecl::accept(Visitor *v)
{
  v->visitBeforeVarDecl(this);
  v->visitVarDecl(this);
  v->visitAfterVarDecl(this);
}


void
ProtoDecl::accept(Visitor *v)
{
  v->visitBeforeProtoDecl(this);
  v->visitProtoDecl(this);
  v->visitAfterProtoDecl(this);
}


void
FuncDecl::accept(Visitor *v)
{
  v->visitBeforeFuncDecl(this);
  if (v->visitFuncDecl(this))
  {
    proto->accept(v);
    body->accept(v);
  }
  v->visitAfterFuncDecl(this);
}


void
ExternFuncDecl::accept(Visitor *v)
{
  v->visitBeforeExternFuncDecl(this);
  if (v->visitExternFuncDecl(this))
  {
    proto->accept(v);
  }
  v->visitAfterExternFuncDecl(this);
}


void
BlockStmt::accept(Visitor *v)
{
  v->visitBeforeBlockStmt(this);
  if (v->visitBlockStmt(this))
  {
    for (auto stmt : stmts)
      stmt->accept(v);
  }
  v->visitAfterBlockStmt(this);
}


void
DeclStmt::accept(Visitor *v)
{
  v->visitBeforeDeclStmt(this);
  if (v->visitDeclStmt(this))
  {
    decl->accept(v);
  }
  v->visitAfterDeclStmt(this);
}


void
ExprStmt::accept(Visitor *v)
{
  v->visitBeforeExprStmt(this);
  if (v->visitExprStmt(this))
  {
    expr->accept(v);
  }
  v->visitAfterExprStmt(this);
}


void
ForStmt::accept(Visitor* v)
{
  v->visitBeforeForStmt(this);
  if (v->visitForStmt(this))
  {
    init->accept(v);
    cond->accept(v);
    step->accept(v);
    body->accept(v);
  }
  v->visitAfterForStmt(this);
}


void
IfStmt::accept(Visitor *v)
{
  v->visitBeforeIfStmt(this);
  if (v->visitIfStmt(this))
  {
    cond->accept(v);
    then->accept(v);
    if (otherwise) otherwise->accept(v);
  }
  v->visitAfterIfStmt(this);
}


void
WhileStmt::accept(Visitor *v)
{
  v->visitBeforeWhileStmt(this);
  if (v->visitWhileStmt(this))
  {
    cond->accept(v);
    body->accept(v);
  }
  v->visitAfterWhileStmt(this);
}


void
UnlessStmt::accept(Visitor *v)
{
  v->visitBeforeUnlessStmt(this);
  if (v->visitUnlessStmt(this))
  {
    cond->accept(v);
    body->accept(v);
  }
  v->visitAfterUnlessStmt(this);
}


void
BreakStmt::accept(Visitor *v)
{
  v->visitBeforeBreakStmt(this);
  v->visitBreakStmt(this);
  v->visitAfterBreakStmt(this);
}


void
ContinueStmt::accept(Visitor *v)
{
  v->visitBeforeContinueStmt(this);
  v->visitContinueStmt(this);
  v->visitAfterContinueStmt(this);
}


void
ReturnStmt::accept(Visitor *v)
{
  v->visitBeforeReturnStmt(this);
  if (v->visitReturnStmt(this) && value)
  {
    value->accept(v);
  }
  v->visitAfterReturnStmt(this);
}


void
LoopStmt::accept(Visitor *v)
{
  v->visitBeforeLoopStmt(this);
  if (v->visitLoopStmt(this))
  {
    body->accept(v);
  }
  v->visitAfterLoopStmt(this);
}


void
VarExpr::accept(Visitor *v)
{
  v->visitBeforeVarExpr(this);
  v->visitVarExpr(this);
  v->visitAfterVarExpr(this);
}


void
IntegerLitExpr::accept(Visitor *v)
{
  v->visitBeforeIntegerLitExpr(this);
  v->visitIntegerLitExpr(this);
  v->visitAfterIntegerLitExpr(this);
}


void
BooleanLitExpr::accept(Visitor *v)
{
  v->visitBeforeBooleanLitExpr(this);
  v->visitBooleanLitExpr(this);
  v->visitAfterBooleanLitExpr(this);
}


void
FloatLitExpr::accept(Visitor *v)
{
  v->visitBeforeFloatLitExpr(this);
  v->visitFloatLitExpr(this);
  v->visitAfterFloatLitExpr(this);
}


void
StringLitExpr::accept(Visitor *v)
{
  v->visitBeforeStringLitExpr(this);
  v->visitStringLitExpr(this);
  v->visitAfterStringLitExpr(this);
}


void
BinOpExpr::accept(Visitor *v)
{
  v->visitBeforeBinOpExpr(this);
  if (v->visitBinOpExpr(this))
  {
    lhs->accept(v);
    rhs->accept(v);
  }
  v->visitAfterBinOpExpr(this);
}


void
UnaryOpExpr::accept(Visitor *v)
{
  v->visitBeforeUnaryOpExpr(this);
  if (v->visitUnaryOpExpr(this))
  {
    expr->accept(v);
  }
  v->visitAfterUnaryOpExpr(this);
}


void
CastExpr::accept(Visitor *v)
{
  v->visitBeforeCastExpr(this);
  if (v->visitCastExpr(this))
  {
    expr->accept(v);
  }
  v->visitAfterCastExpr(this);
}


void
CallExpr::accept(Visitor *v)
{
  v->visitBeforeCallExpr(this);
  if (v->visitCallExpr(this))
  {
    for (auto arg : *args)
      arg->accept(v);
  }
  v->visitAfterCallExpr(this);
}


bool
Visitor::visit(ASTNode *n)
{
  switch (n->getKind())
  {
    case internal::TopLevelDeclId:
      return visitTopLevelDecl(dyncast<TopLevelDecl>(n));
    case internal::VarDeclId:
      return visitVarDecl(dyncast<VarDecl>(n));
    case internal::ProtoDeclId:
      return visitProtoDecl(dyncast<ProtoDecl>(n));
    case internal::FuncDeclId:
      return visitFuncDecl(dyncast<FuncDecl>(n));
    case internal::ExternFuncDeclId:
      return visitExternFuncDecl(dyncast<ExternFuncDecl>(n));
    case internal::BlockStmtId:
      return visitBlockStmt(dyncast<BlockStmt>(n));
    case internal::DeclStmtId:
      return visitDeclStmt(dyncast<DeclStmt>(n));
    case internal::ExprStmtId:
      return visitExprStmt(dyncast<ExprStmt>(n));
    case internal::IfStmtId:
      return visitIfStmt(dyncast<IfStmt>(n));
    case internal::WhileStmtId:
      return visitWhileStmt(dyncast<WhileStmt>(n));
    case internal::UnlessStmtId:
      return visitUnlessStmt(dyncast<UnlessStmt>(n));
    case internal::BreakStmtId:
      return visitBreakStmt(dyncast<BreakStmt>(n));
    case internal::ContinueStmtId:
      return visitContinueStmt(dyncast<ContinueStmt>(n));
    case internal::ReturnStmtId:
      return visitReturnStmt(dyncast<ReturnStmt>(n));
    case internal::LoopStmtId:
      return visitLoopStmt(dyncast<LoopStmt>(n));
    case internal::VarExprId:
      return visitVarExpr(dyncast<VarExpr>(n));
    case internal::IntegerLitExprId:
      return visitIntegerLitExpr(dyncast<IntegerLitExpr>(n));
    case internal::BooleanLitExprId:
      return visitBooleanLitExpr(dyncast<BooleanLitExpr>(n));
    case internal::FloatLitExprId:
      return visitFloatLitExpr(dyncast<FloatLitExpr>(n));
    case internal::BinOpExprId:
      return visitBinOpExpr(dyncast<BinOpExpr>(n));
    case internal::UnaryOpExprId:
      return visitUnaryOpExpr(dyncast<UnaryOpExpr>(n));
    case internal::CastExprId:
      return visitCastExpr(dyncast<CastExpr>(n));
    case internal::CallExprId:
      return visitCallExpr(dyncast<CallExpr>(n));
    default:
      break;
  }
  return false;
}
