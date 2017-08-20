// clang++ app.cc -o app `llvm-config --cxxflags --libs --ldflags core`

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"


int main()
{
  llvm::LLVMContext &context = llvm::getGlobalContext();
  llvm::Module *module = new llvm::Module("text", context);
  llvm::IRBuilder<> builder(context);

  llvm::FunctionType *funcType =
    llvm::FunctionType::get(builder.getInt32Ty(), false);
  llvm::Function *mainFunc =
    llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
  llvm::BasicBlock *entry =
    llvm::BasicBlock::Create(context, "entry", mainFunc);
  builder.SetInsertPoint(entry);

  module->dump();
  return 0;
}
