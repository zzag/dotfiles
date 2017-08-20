#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"


/**
  * Running
  * 
  * $ cl-demo_syntax_only [... arguments] --
  */


using namespace clang;
using namespace clang::tooling;
using namespace llvm;


static llvm::cl::OptionCategory syntaxOnlyCategory("tool options");


int main(int argc, const char **argv) {
    // parse command line arguments
    CommonOptionsParser options(argc, argv, syntaxOnlyCategory);

    // create clang tool instance
    ClangTool tool(options.getCompilations(),
            options.getSourcePathList());

    // run tool
    int retVal = tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());

    return retVal;
}
