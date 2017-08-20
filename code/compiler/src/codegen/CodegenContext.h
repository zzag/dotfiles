#ifndef CODEGEN_CONTEXT_H
#define CODEGEN_CONTEXT_H

#include "SymbolTable.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Type.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "DeclCodegen.h"
#include "StmtCodegen.h"
#include "ExprCodegen.h"
#include "parser/AST.h"
#include <string>


class CodegenContext
{
public:
  CodegenContext();
  ~CodegenContext();

  llvm::Module* getModule() { return module; }
  llvm::IRBuilder<>& getBuilder() { return *builder; }
  DeclCodegen* getDeclCodegen() { return declCodegen; }
  StmtCodegen* getStmtCodegen() { return stmtCodegen; }
  ExprCodegen* getExprCodegen() { return exprCodegen; }
  SymbolTable<std::string, llvm::AllocaInst*>& getSymbolTable() { return variables; }
  SymbolTable<std::string, std::string>& getTypes() { return types; }
  SymbolTable<std::string, ProtoDecl*>& getFuncs() { return funcs; }
  llvm::legacy::FunctionPassManager* getPassManager() { return passManager; }

  void generate(TopLevelDecl* root);

private:
  llvm::Module* module;
  llvm::IRBuilder<>* builder;
  llvm::legacy::FunctionPassManager* passManager;
  DeclCodegen* declCodegen;
  StmtCodegen* stmtCodegen;
  ExprCodegen* exprCodegen;
  SymbolTable<std::string, llvm::AllocaInst*> variables;
  SymbolTable<std::string, std::string> types;
  SymbolTable<std::string, ProtoDecl*> funcs;
};

#endif /* CODEGEN_CONTEXT_H */
