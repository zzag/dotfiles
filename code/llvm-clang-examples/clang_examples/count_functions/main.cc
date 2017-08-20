#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include <cstddef>
#include <iostream>
#include <memory>


/**
  * Usage
  *
  * $ cl-demo_count_functions somefile.cc --
  */


using namespace clang;
using namespace clang::tooling;
using namespace llvm;


// total number of the functions
static size_t numberFunctions = 0;


class CountFunctionVisitor : public RecursiveASTVisitor<CountFunctionVisitor> {
public:
    virtual ~CountFunctionVisitor() {}

    // called for every function
    // in translation unit
    virtual bool VisitFunctionDecl(FunctionDecl *func) {
        numberFunctions++;
        return true;
    }
};


// define AST consumer
class CountFunctionASTConsumer : public ASTConsumer {
public:
    // AST traverse for every translation unit
    virtual void HandleTranslationUnit(ASTContext &context) {
        m_astVisitor.TraverseDecl(context.getTranslationUnitDecl());
    }

private:
    CountFunctionVisitor m_astVisitor;
};


// define frontend action
class CountNumberFunctinFrontendAction : public ASTFrontendAction {
public:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &ci, StringRef file) {
        return std::unique_ptr<ASTConsumer>(new CountFunctionASTConsumer);
    }
};


static llvm::cl::OptionCategory countFunctionCategory("");

int main(int argc, const char **argv) {
    // parse command line arguments
    CommonOptionsParser options(argc, argv, countFunctionCategory);

    // create clang tool instance
    ClangTool tool(
            options.getCompilations(),
            options.getSourcePathList());

    // run tool
    int retVal = tool.run(
            newFrontendActionFactory<CountNumberFunctinFrontendAction>().get());

    // print it
    std::cout << "Number of the functions: " << numberFunctions << std::endl;

    return retVal;
}
