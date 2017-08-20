#include "frontend/CompilerFrontend.h"
#include <iostream>


using std::cout;
using std::endl;


class Printer : public Visitor
{
public:
  Printer() : level(0) {}

  bool visitTopLevelDecl(TopLevelDecl*) override
  {
    cout << "(\033[1;30mTopLevelDecl\033[0m";
    bump();
    return true;
  }

  void visitAfterTopLevelDecl(TopLevelDecl*) override
  {
    back();
    cout << ")" << endl;
  }

  bool visitFuncDecl(FuncDecl *func) override
  {
    std::string ty = func->getProto()->getReturnType();
    space();
    cout << "(\033[1;30mFuncDecl\033[0m ";
    cout << "\033[32m" <<  func->getProto()->getId() << "\033[0m:";
    cout << "\033[31m" << (ty.empty() ? "void" : ty) << "\033[0m";
    bump();
    return true;
  }

  void visitAfterFuncDecl(FuncDecl *func) override
  {
    back();
    cout << ")";
  }

  bool visitVarDecl(VarDecl *var) override
  {
    space();
    cout << "(\033[1;30mVarDecl\033[0m \033[32m" << var->getId() << "\033[0m:\033[31m" << var->getType() << "\033[0m";
    bump();
    if (var->getInitialValue())
      var->getInitialValue()->accept(this);
    back();
    cout << ")";
    return false;
  }

  void visitAfterProtoDecl(ProtoDecl *proto)
  {
    for (auto arg : proto->getArgs())
    {
      space();

      VarDecl* v = reinterpret_cast<VarDecl*>(arg);
      cout << "(\033[1;30mVarDecl\033[0m \033[32m" << v->getId() << "\033[0m:\033[31m" << v->getType() << "\033[0m)";
    }
  }

  bool visitBlockStmt(BlockStmt*)
  {
    space();
    cout << "(\033[1;33mBlockStmt\033[0m";
    bump();
    return true;
  }

  bool visitExternFuncDecl(ExternFuncDecl* func)
  {
    std::string ty = func->getProto()->getReturnType();
    space();
    cout << "(\033[1;30mExternFuncDecl\033[0m ";
    cout << "\033[32m" << func->getProto()->getId() << "\033[0m:";
    cout << "\033[31m" << (ty.empty() ? "void" : ty) << "\033[0m";
    bump();
    return true;
  }

  void visitAfterExternFuncDecl(ExternFuncDecl*)
  {
    back();
    cout << ")";
  }

  void visitAfterBlockStmt(BlockStmt* s)
  {
    back();
    cout << ")";
  }

  bool visitIfStmt(IfStmt*)
  {
    space();
    cout << "(\033[1;33mIfStmt\033[0m";
    bump();
    return true;
  }

  void visitAfterIfStmt(IfStmt*)
  {
    back();
    cout << ")";
  }

  bool visitForStmt(ForStmt*)
  {
    space();
    cout << "(\033[1;33mForStmt\033[0m";
    bump();
    return true;
  }

  void visitAfterForStmt(ForStmt*)
  {
    back();
    cout << ")";
  }

  bool visitWhileStmt(WhileStmt*)
  {
    space();
    cout << "(\033[1;33mWhileStmt\033[0m";
    bump();
    return true;
  }

  void visitAfterWhileStmt(WhileStmt*)
  {
    back();
    cout << ")";
  }

  bool visitUnlessStmt(UnlessStmt*)
  {
    space();
    cout << "(\033[1;33mUnlessStmt\033[0m";
    bump();
    return true;
  }

  void visitAfterUnlessStmt(UnlessStmt*)
  {
    back();
    cout << ")";
  }

  bool visitLoopStmt(LoopStmt*)
  {
    space();
    cout << "(\033[1;33mLoopStmt\033[0m";
    bump();
    return true;
  }

  void visitAfterLoopStmt(LoopStmt*)
  {
    back();
    cout << ")";
  }

  void visitAfterBreakStmt(BreakStmt*)
  {
    space();
    cout << "(\033[1;33mBreakStmt\033[0m)";
  }

  void visitAfterContinueStmt(ContinueStmt*)
  {
    space();
    cout << "(\033[1;33mContinueStmt\033[0m)";
  }

  bool visitReturnStmt(ReturnStmt*)
  {
    space();
    cout << "(\033[1;33mReturnStmt\033[0m";
    bump();
    return true;
  }

  void visitAfterReturnStmt(ReturnStmt*)
  {
    back();
    cout << ")";
  }

  bool visitDeclStmt(DeclStmt*)
  {
    space();
    cout << "(\033[1;33mDeclStmt\033[0m";
    bump();
    return true;
  }

  void visitAfterDeclStmt(DeclStmt*)
  {
    back();
    cout << ")";
  }

  bool visitExprStmt(ExprStmt*)
  {
    space();
    cout << "(\033[1;33mExprStmt\033[0m";
    bump();
    return true;
  }

  void visitAfterExprStmt(ExprStmt*)
  {
    back();
    cout << ")";
  }

  void visitAfterVarExpr(VarExpr *var)
  {
    space();
    cout << "(\033[1;36mVarExpr\033[0m \'\033[32m" << var->getId() << "\033[0m\')";
  }

  void visitAfterIntegerLitExpr(IntegerLitExpr *lit)
  {
    space();
    cout << "(\033[1;36mIntegerLitExpr\033[0m \'" << lit->getValue() << "\')";
  }

  void visitAfterBooleanLitExpr(BooleanLitExpr *lit)
  {
    space();
    cout << "(\033[1;36mBooleanLitExpr\033[0m \'" << lit->getValue() << "\')";
  }

  void visitAfterFloatLitExpr(FloatLitExpr *lit)
  {
    space();
    cout << "(\033[1;36mFloatLitExpr\033[0m \'" << lit->getValue() << "\')";
  }

  void visitAfterStringLitExpr(StringLitExpr *lit)
  {
    space();
    cout << "(\033[1;36mStringLitExpr\033[0m \'" << lit->getValue() << "\')";
  }

  bool visitBinOpExpr(BinOpExpr *e)
  {
    std::string op = "";
    switch (e->getOp())
    {
      case ops::Add: op = "+"; break;
      case ops::Sub: op = "-"; break;
      case ops::Mul: op = "*"; break;
      case ops::Div: op = "/"; break;
      case ops::Mod: op = "%"; break;
      case ops::Shl: op = "<<"; break;
      case ops::Shr: op = ">>"; break;
      case ops::BitAnd: op = "&"; break;
      case ops::BitOr: op = "|"; break;
      case ops::BitXor: op = "^"; break;
      case ops::Assign: op = "="; break;
      case ops::Lt: op = "<"; break;
      case ops::Gt: op = ">"; break;
      case ops::Le: op = "<="; break;
      case ops::Ge: op = ">="; break;
      case ops::Eq: op = "=="; break;
      case ops::Ne: op = "!="; break;
      case ops::And: op = "&&"; break;
      case ops::Or: op = "||"; break;
      default:
        break;
    }
    space();
    cout << "(\033[1;36mBinOpExpr\033[0m \'" << op << "\'";
    bump();
    return true;
  }

  bool visitUnaryOpExpr(UnaryOpExpr* e)
  {
    std::string op = "";
    switch (e->getOp())
    {
      case ops::Not: op = "!"; break;
      case ops::Neg: op = "-"; break;
      case ops::PrefixDec: op = "prefix --"; break;
      case ops::PrefixInc: op = "prefix ++"; break;
      case ops::PostfixDec: op = "postfix --"; break;
      case ops::PostfixInc: op = "postfix ++"; break;
      default:
        break;
    }
    space();
    cout << "(\033[1;36mUnaryOpExpr\033[0m \'" << op << "\'";
    bump();
    return true;
  }

  void visitAfterUnaryOpExpr(UnaryOpExpr*)
  {
    back();
    cout << ")";
  }

  void visitAfterBinOpExpr(BinOpExpr*)
  {
    back();
    cout << ")";
  }

  bool visitCastExpr(CastExpr *e)
  {
    space();
    cout << "(\033[1;36mCastExpr\033[0m to \'\033[31m" << e->getType() << "\033[0m\'";
    bump();
    return true;
  }

  void visitAfterCastExpr(CastExpr*)
  {
    back();
    cout << ")";
  }

  bool visitCallExpr(CallExpr *call)
  {
    space();
    cout << "(\033[1;36mCallExpr\033[0m \'\033[32m" << call->getId() << "\033[0m\'";
    bump();
    return true;
  }

  void visitAfterCallExpr(CallExpr*)
  {
    back();
    cout << ")";
  }

private:
  void bump() { level += 2; }
  void back() { level -= 2; }
  void space()
  {
    cout << endl;
    for (int i = 0; i < level; i++)
    {
      cout << ' ';
    }
  }

private:
  int level;
};


int main(int argc, char **argv)
{
  std::string filename = argv[1];
  CompilerFrontend frontend(filename);
  TopLevelDecl* root = frontend.buildAST();
  if (! root) return -1;


  Printer printer;
  root->accept(&printer);

  return 0;
}
