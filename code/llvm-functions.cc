#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"


using namespace llvm;


Function *CreateSumFunction(IRBuilder<> &builder, Module *module)
{
  Type *args[] = { builder.getInt32Ty(), builder.getInt32Ty() };
  FunctionType *funcType = FunctionType::get(builder.getInt32Ty(), args, false);
  Function *func = Function::Create(funcType, Function::ExternalLinkage, "sum", module);
  BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", func);
  builder.SetInsertPoint(BB);

  Function::arg_iterator argsIterator = func->arg_begin();
  Value *a = argsIterator++;
  a->setName("a");
  Value *b = argsIterator++;
  b->setName("b");

  Value *c = builder.CreateAdd(a, b, "c");
  builder.CreateRet(c);

  return func;
}


Constant *GetPutcharFunction(IRBuilder<> &builder, Module *module)
{
  Type *args[] = { builder.getInt32Ty() };
  FunctionType *funcType = FunctionType::get(builder.getInt32Ty(), args, false);
  return module->getOrInsertFunction("putchar", funcType);
}


int main()
{
  LLVMContext &context = getGlobalContext();
  Module *module = new Module("blah-blah", context);
  IRBuilder<> builder(context);
  
  Function *sum = CreateSumFunction(builder, module);
  Constant *putchar = GetPutcharFunction(builder, module);

  // create main
  FunctionType *mainType = FunctionType::get(builder.getInt32Ty(), false);
  Function *mainFunc = Function::Create(mainType, Function::ExternalLinkage, "main", module);
  BasicBlock *entry = BasicBlock::Create(context, "entry", mainFunc);
  builder.SetInsertPoint(entry);

  Value *args[] = { ConstantInt::get(context, APInt(32, 48)),
                    ConstantInt::get(context, APInt(32, 5)) };
  Value *c = builder.CreateCall(sum, args);

  builder.CreateCall(putchar, c);

  Value *status = ConstantInt::get(context, APInt(32, 0));
  builder.CreateRet(status);

  module->dump();
  return 0;
}
