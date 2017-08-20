#ifndef TYPES_PASS_H
#define TYPES_PASS_H

#include "Pass.h"
#include "codegen/SymbolTable.h"
#include <string>


class TypesPass : public Pass
{
public:
  TypesPass();
  ~TypesPass();

  //
  // top level decl - introduce a new scope
  //
  bool visitTopLevelDecl(TopLevelDecl* d) override;
  void visitAfterTopLevelDecl(TopLevelDecl* d) override;

  //
  // var declaration
  //
  bool visitVarDecl(VarDecl* var) override;

  //
  // func declaration
  //
  bool visitFuncDecl(FuncDecl* func) override;
  bool visitExternFuncDecl(ExternFuncDecl* func) override;

  //
  // visit block statement
  //
  bool visitBlockStmt(BlockStmt* b) override;
  void visitAfterBlockStmt(BlockStmt* b) override;

  //
  // CFG
  //
  bool visitIfStmt(IfStmt* s) override;
  bool visitForStmt(ForStmt* s) override;
  bool visitWhileStmt(WhileStmt* s) override;
  bool visitUnlessStmt(UnlessStmt* s) override;
  bool visitLoopStmt(LoopStmt* s) override;
  bool visitReturnStmt(ReturnStmt* r) override;

  //
  // var expr
  //
  bool visitVarExpr(VarExpr* var) override;

  //
  // literals
  //
  bool visitIntegerLitExpr(IntegerLitExpr* lit) override;
  bool visitBooleanLitExpr(BooleanLitExpr* lit) override;
  bool visitFloatLitExpr(FloatLitExpr* lit) override;
  bool visitStringLitExpr(StringLitExpr* lit) override;

  //
  // binary expression
  //
  bool visitBinOpExpr(BinOpExpr* e) override;

  //
  // unary expression
  //
  bool visitUnaryOpExpr(UnaryOpExpr* e) override;

  //
  // cast expression
  //
  bool visitCastExpr(CastExpr* e) override;

  //
  // call expression
  //
  bool visitCallExpr(CallExpr* e) override;

  bool isValidType(std::string type)
  {
    if (type == "i32") return true;
    else if (type == "f32") return true;
    else if (type == "bool") return true;
    else if (type == "str") return true;
    else if (type == "") return true;

    error("unknown type: " + type);
    return false;
  }

  bool areTypesValid(std::string expected, std::string actual)
  {
    bool ok = expected == actual;
    if (! ok)
      error("type mismatch expected \'" + expected + "\' but found \'" + actual + "\'");
    return ok;
  }

  bool areTypesValid(ops::BinaryOp op, std::string expected, std::string actual)
  {
    bool ok = expected == actual;
    if (! ok)
      error("type mismatch expected \'" + expected + "\' but found \'" + actual + "\'");
    if (actual == "str" && op != ops::Assign)
      error("not allowed action on string type");
    return ok;
  }

private:
  SymbolTable<std::string, std::string> symbols;
  SymbolTable<std::string, ProtoDecl*> funcs;
  std::string lastType;
  ProtoDecl* lastProto;
  bool hasReturnStmt;
};

#endif /* TYPES_PASS_H */
