#include "CompilerFrontend.h"
#include "parser/Parser.h"
#include "parser/FileLoader.h"
#include "sema/Sema.h"
#include "sema/TypesPass.h"
#include "sema/CFGPass.h"
#include "sema/TerminatorPass.h"
#include "codegen/CodegenContext.h"


CompilerFrontend::CompilerFrontend(std::string& filename) :
    filename(filename)
{
}


CompilerFrontend::~CompilerFrontend()
{
}


TopLevelDecl*
CompilerFrontend::buildAST()
{
  std::string buffer = "";

  if (! FileLoader::load(filename, buffer))
    return nullptr;

  Parser parser;
  TopLevelDecl *root = parser.parse(buffer);

  Sema sema;
  sema.add(new TypesPass());
  sema.add(new CFGPass());
  sema.add(new TerminatorPass());

  if (! sema.isValid(root))
    return nullptr;

  return root;
}


llvm::Module*
CompilerFrontend::codegen()
{
  TopLevelDecl* root = buildAST();
  if (! root) return nullptr;

  CodegenContext codegen;
  codegen.generate(root);

  return codegen.getModule();
}


void
CompilerFrontend::emitIR()
{
  llvm::Module* module = codegen();
  if (! module) return;
  module->dump();
}
