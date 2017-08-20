#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"


//
//
// Simple example
//  - Create main()
//  - AllocaInst
//  - StoreInst
//
// Run:
// bin/ll-demo_simple_main
//


int main() {
	llvm::LLVMContext &context = llvm::getGlobalContext();
	llvm::Module *module = new llvm::Module("main", context);
	llvm::IRBuilder<> builder(context);


	// declare main
	llvm::FunctionType *functionType = 
		llvm::FunctionType::get(builder.getInt32Ty(), false);
	llvm::Function *mainFunction = 
		llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "main", module);
	llvm::BasicBlock *entrypoint = 
		llvm::BasicBlock::Create(context, "entrypoint", mainFunction);
	builder.SetInsertPoint(entrypoint);

	//
	// append some code at the end of entrypoint
	// 
	 
	// alloc i32
	llvm::Instruction *ai = new llvm::AllocaInst(builder.getInt32Ty(), 0, "varname");
	builder.GetInsertBlock()->getInstList().push_back(ai);

	// another way:
	// builder.Insert(ai);

	// store some value
	llvm::Value *ni = llvm::ConstantInt::get(
		builder.getInt32Ty(), 567);
	llvm::Instruction *si = new llvm::StoreInst(
		ni, ai, false);
	builder.GetInsertBlock()->getInstList().push_back(si);

	// another way:
	// builder.Insert(si);

	module->dump();
	delete module;
	return 0;
}