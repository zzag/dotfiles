#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"


using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace llvm;

//
//
// $ cl-demo_ast_matchers tst.cc --
//
//


class SomeHandler : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &result) {
        const IfStmt *stmt = result.Nodes.getNodeAs<IfStmt>("if_stmt");
        stmt->dump();
    }
};


static llvm::cl::OptionCategory toolCategory("ast_matcher_ex");


int main (int argc, const char **argv) {
	CommonOptionsParser options(argc, argv, toolCategory);
	ClangTool tool(options.getCompilations(),
		options.getSourcePathList());
	SomeHandler someHandler;
	MatchFinder finder;

    // http://clang.llvm.org/docs/LibASTMatchersReference.html
	StatementMatcher matcher = ifStmt(hasCondition(boolLiteral(equals(true)))).bind("if_stmt");

	finder.addMatcher(matcher, &someHandler);

	int retVal = tool.run(newFrontendActionFactory(&finder).get());
	return retVal;
}
