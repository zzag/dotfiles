#include "TypesPass.h"


TypesPass::TypesPass() : lastType(""), lastProto(nullptr)
{
}


TypesPass::~TypesPass()
{
}


bool
TypesPass::visitTopLevelDecl(TopLevelDecl* d)
{
  symbols.pushBlock();
  funcs.pushBlock();
  return true;
}


void
TypesPass::visitAfterTopLevelDecl(TopLevelDecl* d)
{
  symbols.popBlock();
  funcs.popBlock();
}


bool
TypesPass::visitVarDecl(VarDecl* var)
{
  if (symbols.currentBlock().exists(var->getId()))
    error("redeclaration of " + var->getId());

  if (var->getInitialValue())
  {
    var->getInitialValue()->accept(this);
    if (var->getType().empty())
      var->setType(lastType);
    else
      areTypesValid(var->getType(), lastType);
  }

  isValidType(var->getType());
  symbols.declare(var->getId(), var->getType());

  return false;
}


bool
TypesPass::visitFuncDecl(FuncDecl* func)
{
  ProtoDecl* proto = func->getProto();

  if (lastProto)
  {
    error("declare func at top level");
    return false;
  }

  if (funcs.exists(proto->getId()))
      error("redeclaration of " + proto->getId());
  funcs.declare(proto->getId(), proto);

  isValidType(proto->getReturnType());
  symbols.pushBlock();
  for (auto arg : proto->getArgs())
    arg->accept(this);

  lastProto = proto;
  hasReturnStmt = false;
  func->getBody()->accept(this);
  lastProto = nullptr;

  if (! proto->getReturnType().empty() && hasReturnStmt == false)
    error("no return statement");

  symbols.popBlock();
  return false;
}


bool
TypesPass::visitExternFuncDecl(ExternFuncDecl* func)
{
  ProtoDecl* proto = func->getProto();

  if (lastProto)
  {
    error("declare func at top level");
    return false;
  }

  if (funcs.exists(proto->getId()))
      error("redeclaration of " + proto->getId());
  funcs.declare(proto->getId(), proto);

  isValidType(proto->getReturnType());
  symbols.pushBlock();
  for (auto arg : proto->getArgs())
    arg->accept(this);
  symbols.popBlock();
  lastProto = nullptr;
  return false;
}


bool
TypesPass::visitBlockStmt(BlockStmt*)
{
  symbols.pushBlock();
  return true;
}


bool
TypesPass::visitIfStmt(IfStmt* s)
{
  s->getCond()->accept(this);
  if (lastType != "bool") error("if: condition is not boolean");
  s->getThen()->accept(this);
  if (s->getOtherwise()) s->getOtherwise()->accept(this);
  return false;
}


bool
TypesPass::visitForStmt(ForStmt* s)
{
  symbols.pushBlock();
  s->getInit()->accept(this);
  s->getCond()->accept(this);
  if (lastType != "bool") error("for: condition is not boolean");
  s->getStep()->accept(this);
  s->getBody()->accept(this);
  symbols.popBlock();
  return false;
}


bool
TypesPass::visitWhileStmt(WhileStmt* s)
{
  s->getCond()->accept(this);
  if (lastType != "bool") error("while: condition is not boolean");
  s->getBody()->accept(this);
  return false;
}


bool
TypesPass::visitUnlessStmt(UnlessStmt* s)
{
  s->getCond()->accept(this);
  if (lastType != "bool") error("unless: condition is not boolean");
  s->getBody()->accept(this);
  return false;
}


bool
TypesPass::visitLoopStmt(LoopStmt* s)
{
  return true;
}


bool
TypesPass::visitReturnStmt(ReturnStmt* r)
{
  if (! lastProto)
  {
    error("return statement is outside of func");
    return false;
  }

  std::string retType = "";
  if (r->getValue())
  {
    r->getValue()->accept(this);
    retType = lastType;
  }

  areTypesValid(lastProto->getReturnType(), retType);
  hasReturnStmt = true;
  return false;
}


void
TypesPass::visitAfterBlockStmt(BlockStmt*)
{
  symbols.popBlock();
}


bool
TypesPass::visitVarExpr(VarExpr* e)
{
  if (! symbols.exists(e->getId()))
  {
    error("unknown identifier " + e->getId());
    lastType = "";
  }
  else
    lastType = symbols.get(e->getId());
  return false;
}


bool
TypesPass::visitIntegerLitExpr(IntegerLitExpr*)
{
  lastType = "i32";
  return false;
}


bool
TypesPass::visitBooleanLitExpr(BooleanLitExpr*)
{
  lastType = "bool";
  return false;
}


bool
TypesPass::visitFloatLitExpr(FloatLitExpr*)
{
  lastType = "f32";
  return false;
}


bool
TypesPass::visitStringLitExpr(StringLitExpr*)
{
  lastType = "str";
  return false;
}


bool
TypesPass::visitBinOpExpr(BinOpExpr* e)
{
  e->getLHS()->accept(this);
  std::string lhsType = lastType;

  e->getRHS()->accept(this);
  std::string rhsType = lastType;

  if (! areTypesValid(e->getOp(), lhsType, rhsType))
  {
    lastType = "";
    return false;
  }
  
  switch (e->getOp())
  {
    case ops::Shl:
    case ops::Shr:
    case ops::BitAnd:
    case ops::BitOr:
    case ops::BitXor:
      if (lhsType != "i32") error("float arguments are illegal");
      break;
    case ops::Lt:
    case ops::Le:
    case ops::Gt:
    case ops::Ge:
    case ops::Eq:
    case ops::Ne:
      lastType = "bool";
      break;
    case ops::And:
    case ops::Or:
      if (lhsType != "bool") error("expected boolean type");
      break;
    default:
      lastType = lhsType;
      break;
  }

  return false;
}


bool
TypesPass::visitUnaryOpExpr(UnaryOpExpr* e)
{
  e->getExpr()->accept(this);

  if (lastType == "str") error("not allowed action on string");

  switch (e->getOp())
  {
    case ops::Not:
      lastType = "bool";
      return false;
    case ops::PrefixInc:
    case ops::PrefixDec:
    case ops::PostfixInc:
    case ops::PostfixDec:
      if (! isa<VarExpr>(e->getExpr())) error("is not assignable");
      return false;
    default:
      return true;
  }
}


bool
TypesPass::visitCastExpr(CastExpr* e)
{
  e->getExpr()->accept(this);
  if (lastType == "str") error("unable to cast from string");
  lastType = e->getType();
  if (lastType == "str") error("unable to cast to string");
  return false;
}


bool
TypesPass::visitCallExpr(CallExpr* e)
{
  if (! funcs.exists(e->getId()))
  {
    error("unknown func");
    return false;
  }

  ProtoDecl* proto = funcs.get(e->getId());
  DeclList& funcArgs = proto->getArgs();
  ExprList& callArgs = e->getArgs();

  if (funcArgs.size() != callArgs.size())
    error("invalid number of arguments");

  for (size_t i = 0; i < funcArgs.size(); i++)
  {
    VarDecl* funcArg = reinterpret_cast<VarDecl*>(funcArgs.at(i));
    Expr* callArg = callArgs.at(i);

    callArg->accept(this);
    areTypesValid(funcArg->getType(), lastType);
  }

  lastType = proto->getReturnType();
  return false;
}
