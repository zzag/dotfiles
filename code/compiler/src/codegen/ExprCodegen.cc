#include "ExprCodegen.h"
#include "CodegenContext.h"


using namespace llvm;


bool
ExprCodegen::visitVarExpr(VarExpr* e)
{
  AllocaInst* alloca = context->getSymbolTable().get(e->getId());
  lastValue = context->getBuilder().CreateLoad(alloca);
  lastType = context->getTypes().get(e->getId());
  return false;
}


bool
ExprCodegen::visitIntegerLitExpr(IntegerLitExpr* e)
{
  lastValue = ConstantInt::get(getGlobalContext(), APInt(32, e->getValue(), 10));
  lastType = "i32";
  return false;
}


bool
ExprCodegen::visitBooleanLitExpr(BooleanLitExpr* e)
{
  lastValue = ConstantInt::get(getGlobalContext(), APInt(1, e->getValue()));
  lastType = "bool";
  return false;
};


bool
ExprCodegen::visitFloatLitExpr(FloatLitExpr* e)
{
  lastValue = ConstantFP::get(getGlobalContext(), APFloat(APFloat::IEEEsingle, e->getValue()));
  lastType = "f32";
  return false;
}


bool
ExprCodegen::visitStringLitExpr(StringLitExpr* e)
{
  IRBuilder<>& builder = context->getBuilder();
  lastValue = builder.CreateGlobalStringPtr(e->getValue());
  lastType = "str";
  return false;
}


bool
ExprCodegen::visitCastExpr(CastExpr* e)
{
  e->getExpr()->accept(this);
  Value* v = lastValue;
  std::string fromType = lastType;
  std::string toType = e->getType();

  if (fromType != toType)
  {
    if (fromType == "i32") lastValue = castFromI32To(v, toType);
    else if (fromType == "f32") lastValue = castFromF32To(v, toType);
    lastType = toType;
  }

  return false;
}


Value*
ExprCodegen::castFromI32To(Value* v, std::string toType)
{
  IRBuilder<>& builder = context->getBuilder();

  if (toType == "f32") return builder.CreateSIToFP(v, getType(toType));

  return nullptr;
}


Value*
ExprCodegen::castFromF32To(Value* v, std::string toType)
{
  IRBuilder<>& builder = context->getBuilder();

  if (toType == "i32") return builder.CreateFPToSI(v, getType(toType));

  return nullptr;
}


bool
ExprCodegen::visitBinOpExpr(BinOpExpr* e)
{
  e->getRHS()->accept(this);
  Value* rhs = lastValue;

  if (e->getOp() == ops::Assign)
  {
    VarExpr* lhs = reinterpret_cast<VarExpr*>(e->getLHS());
    lastValue = assignOp(lhs, rhs);
    return false;
  }

  e->getLHS()->accept(this);
  Value* lhs = lastValue;
  std::string type = lastType;

  if (type == "i32") lastValue = integerBinOp(e->getOp(), lhs, rhs);
  else if (type == "bool") lastValue = booleanBinOp(e->getOp(), lhs,rhs);
  else if (type == "f32") lastValue = floatBinOp(e->getOp(), lhs, rhs);

  return false;
}


Value*
ExprCodegen::assignOp(VarExpr* lhs, Value* rhs)
{
  AllocaInst* alloca = context->getSymbolTable().get(lhs->getId());
  context->getBuilder().CreateStore(rhs, alloca);
  return rhs;
}


Value*
ExprCodegen::integerBinOp(ops::BinaryOp op, Value* lhs, Value* rhs)
{
  IRBuilder<>& builder = context->getBuilder();
  switch (op)
  {
    case ops::Add: return builder.CreateAdd(lhs, rhs);
    case ops::Sub: return builder.CreateSub(lhs, rhs);
    case ops::Mul: return builder.CreateMul(lhs, rhs);
    case ops::Div: return builder.CreateSDiv(lhs, rhs);
    case ops::Mod: return builder.CreateSRem(lhs, rhs);
    case ops::Shl: return builder.CreateShl(lhs, rhs);
    case ops::Shr: return builder.CreateLShr(lhs, rhs);
    case ops::BitAnd: return builder.CreateAnd(lhs, rhs);
    case ops::BitOr: return builder.CreateOr(lhs, rhs);
    case ops::BitXor: return builder.CreateXor(lhs,rhs);
    case ops::Lt: lastType = "bool"; return builder.CreateICmpSLT(lhs, rhs);
    case ops::Le: lastType = "bool"; return builder.CreateICmpSLE(lhs, rhs);
    case ops::Gt: lastType = "bool"; return builder.CreateICmpSGT(lhs, rhs);
    case ops::Ge: lastType = "bool"; return builder.CreateICmpSGE(lhs, rhs);
    case ops::Eq: lastType = "bool"; return builder.CreateICmpEQ(lhs, rhs);
    case ops::Ne: lastType = "bool"; return builder.CreateICmpNE(lhs,rhs);
    default: return nullptr;
  }
}


Value*
ExprCodegen::booleanBinOp(ops::BinaryOp op, Value* lhs, Value* rhs)
{
  IRBuilder<>& builder = context->getBuilder();
  switch (op)
  {
    case ops::Add: return builder.CreateAdd(lhs, rhs);
    case ops::Sub: return builder.CreateSub(lhs, rhs);
    case ops::Mul: return builder.CreateMul(lhs, rhs);
    case ops::Div: return builder.CreateSDiv(lhs, rhs);
    case ops::Mod: return builder.CreateSRem(lhs, rhs);
    case ops::Shl: return builder.CreateShl(lhs, rhs);
    case ops::Shr: return builder.CreateLShr(lhs, rhs);
    case ops::BitAnd: return builder.CreateAnd(lhs, rhs);
    case ops::BitOr: return builder.CreateOr(lhs, rhs);
    case ops::BitXor: return builder.CreateXor(lhs,rhs);
    case ops::Lt: return builder.CreateICmpSLT(lhs, rhs);
    case ops::Le: return builder.CreateICmpSLE(lhs, rhs);
    case ops::Gt: return builder.CreateICmpSGT(lhs, rhs);
    case ops::Ge: return builder.CreateICmpSGE(lhs, rhs);
    case ops::Eq: return builder.CreateICmpEQ(lhs, rhs);
    case ops::Ne: return builder.CreateICmpNE(lhs,rhs);
    case ops::And: return builder.CreateAnd(lhs, rhs);
    case ops::Or: return builder.CreateOr(lhs, rhs);
    default:
      return nullptr;
  }
}


Value*
ExprCodegen::floatBinOp(ops::BinaryOp op, Value* lhs, Value* rhs)
{
  IRBuilder<>& builder = context->getBuilder();
  switch (op)
  {
    case ops::Add: return builder.CreateFAdd(lhs, rhs);
    case ops::Sub: return builder.CreateFSub(lhs, rhs);
    case ops::Mul: return builder.CreateFMul(lhs, rhs);
    case ops::Div: return builder.CreateFDiv(lhs, rhs);
    case ops::Mod: return builder.CreateFRem(lhs, rhs);
    case ops::Lt: lastType = "bool"; return builder.CreateFCmpULT(lhs, rhs);
    case ops::Le: lastType = "bool"; return builder.CreateFCmpULE(lhs, rhs);
    case ops::Gt: lastType = "bool"; return builder.CreateFCmpUGT(lhs, rhs);
    case ops::Ge: lastType = "bool"; return builder.CreateFCmpUGE(lhs, rhs);
    case ops::Eq: lastType = "bool"; return builder.CreateFCmpUEQ(lhs, rhs);
    case ops::Ne: lastType = "bool"; return builder.CreateFCmpUNE(lhs, rhs);
    default: return nullptr;
  }
}


void
ExprCodegen::createIncDec(UnaryOpExpr* e, Value* oldValue)
{
  ops::UnaryOp op = e->getOp();
  IRBuilder<>& builder = context->getBuilder();
  bool isPrefix = op == ops::PrefixInc || op == ops::PrefixDec;
  bool isFP = lastType == "f32";
  Type* ty = getType(lastType);
  AllocaInst* alloca = context->getSymbolTable().
    get(reinterpret_cast<VarExpr*>(e->getExpr())->getId());

  // step
  Value* one = isFP ? ConstantFP::get(ty, 1.0) : ConstantInt::get(ty, 1);
  Value* newValue = nullptr;

  // change value
  switch (op)
  {
    case ops::PostfixInc:
    case ops::PrefixInc: 
      newValue = isFP ? builder.CreateFAdd(oldValue, one) :
                        builder.CreateAdd(oldValue, one);
      break;
    case ops::PostfixDec:
    case ops::PrefixDec:
      newValue = isFP ? builder.CreateFSub(oldValue, one) :
                        builder.CreateSub(oldValue, one);
      break;
    default:
      break;
  }

  // store new value
  builder.CreateStore(newValue, alloca);

  lastValue = isPrefix ? newValue : oldValue;
}


bool
ExprCodegen::visitUnaryOpExpr(UnaryOpExpr* e)
{
  IRBuilder<>& builder = context->getBuilder();
  SymbolTable<std::string, AllocaInst*>& vars = context->getSymbolTable();
  e->getExpr()->accept(this);
  Value* value = getValue();

  switch (e->getOp())
  {
    case ops::Not:
      lastValue = builder.CreateNot(value);
      break;
    case ops::Neg:
      if (lastType == "f32") lastValue = builder.CreateFNeg(value);
      else lastValue = builder.CreateNeg(value);
      break;
    case ops::PrefixInc:
    case ops::PrefixDec:
    case ops::PostfixInc:
    case ops::PostfixDec:
      createIncDec(e, value);
      break;
    default:
      break;
  }
  return false;
}


bool
ExprCodegen::visitCallExpr(CallExpr* e)
{
  IRBuilder<>& builder = context->getBuilder();
  Module* module = context->getModule();
  ProtoDecl* proto = context->getFuncs().get(e->getId());

  Function* f = module->getFunction(e->getId());
  std::vector<Value*> args;

  for (auto arg : e->getArgs())
  {
    arg->accept(this);
    args.push_back(lastValue);
  }

  lastValue = builder.CreateCall(f, args);
  lastType = proto->getReturnType();

  return false;
}
