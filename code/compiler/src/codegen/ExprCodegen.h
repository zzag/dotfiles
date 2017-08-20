#ifndef EXPR_CODEGEN_H
#define EXPR_CODEGEN_H

#include "Codegen.h"
#include "llvm/IR/Value.h"


class ExprCodegen : public Codegen
{
public:
  ExprCodegen(CodegenContext* context) : 
      lastValue(nullptr), lastType(""), Codegen(context)
  {
  }
  ~ExprCodegen() {}

  llvm::Value* getValue() { return lastValue; }

  bool visitVarExpr(VarExpr* e) override;
  bool visitIntegerLitExpr(IntegerLitExpr* e) override;
  bool visitBooleanLitExpr(BooleanLitExpr* e) override;
  bool visitFloatLitExpr(FloatLitExpr* e) override;
  bool visitStringLitExpr(StringLitExpr* e) override;
  bool visitCastExpr(CastExpr* e) override;
  bool visitBinOpExpr(BinOpExpr* e) override;
  bool visitUnaryOpExpr(UnaryOpExpr* e) override;
  bool visitCallExpr(CallExpr* e) override;

private:
  llvm::Value* assignOp(VarExpr* lhs, llvm::Value* rhs);
  llvm::Value* integerBinOp(ops::BinaryOp op, llvm::Value* lhs, llvm::Value* rhs);
  llvm::Value* booleanBinOp(ops::BinaryOp op, llvm::Value* lhs, llvm::Value* rhs);
  llvm::Value* floatBinOp(ops::BinaryOp op, llvm::Value* lhs, llvm::Value* rhs);
  llvm::Value* castFromI32To(llvm::Value* v, std::string toType);
  llvm::Value* castFromF32To(llvm::Value* v, std::string toType);
  void createIncDec(UnaryOpExpr* e, llvm::Value* oldValue);

private:
  llvm::Value* lastValue;
  std::string lastType;
};

#endif /* EXPR_CODEGEN_H */
