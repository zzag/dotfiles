#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include <memory>
#include <cctype>


using namespace clang;
using namespace clang::tooling;
using namespace llvm;


/**
 *
 * Execute:
 *  $ cl-demo_rewriter cool_stuff_is_here.cc --
 *
 */


class RewriteExampleASTVisitor : public RecursiveASTVisitor<RewriteExampleASTVisitor> {
public:
    // constructor & destructor
    RewriteExampleASTVisitor(Rewriter &rewriter) :
        m_rewriter(rewriter) {
    }

    virtual ~RewriteExampleASTVisitor() {}


    // visit function decl
    virtual bool VisitFunctionDecl(FunctionDecl *decl) {
        // get function name
        std::string functionName = decl->getNameInfo().getAsString();
        
        if (functionName == "magicFunction") {
            handleMagicFunction(decl);
            return true;
        }

        // if its name is not capitalized then make it capitalized
        if (islower(functionName[0])) {
            functionName[0] = toupper(functionName[0]);

            // replace function name...
            m_rewriter.ReplaceText(decl->getNameInfo().getSourceRange(),
                    functionName);
        }

        return true;
    }

    void handleMagicFunction(FunctionDecl *decl) {
        // remove it...
        Rewriter::RewriteOptions options;
        options.RemoveLineIfEmpty = true;

        m_rewriter.RemoveText(decl->getSourceRange(), options);
    }

    // visit while loop
    bool VisitStmt(Stmt *stmt) {
        if (isa<WhileStmt>(stmt)) {
            // cast to the WhileStmt
            WhileStmt *whileStmt = cast<WhileStmt>(stmt);

            // prepend comment
            // false value indicate whether insert after given location or before
            // true value indicate whether insert new line and then given text or not
            m_rewriter.InsertText(whileStmt->getLocStart(), "// stupid loop\n", false, true);

            // make bad things with body
            Stmt *body = whileStmt->getBody();
            m_rewriter.ReplaceText(body->getSourceRange(), "{}");
        }

        return true;
    }

private:
    Rewriter &m_rewriter;
};


class RewriterExampleASTConsumer : public ASTConsumer {
public:
    // constructor & destructor
    RewriterExampleASTConsumer(Rewriter &rewriter) :
        m_visitor(rewriter) {
    }

    virtual ~RewriterExampleASTConsumer() {}


    virtual void HandleTranslationUnit(ASTContext &context) {
        m_visitor.TraverseDecl(context.getTranslationUnitDecl());
    }

private:
    RewriteExampleASTVisitor m_visitor;
};


class RewriterExampleFrontendAction : public ASTFrontendAction {
public:
    //
    // callback at the start of processing
    // a single input
    //
    virtual bool BeginSourceFileAction(
            CompilerInstance &ci,
            StringRef filename) {

        m_rewriter.setSourceMgr(
                ci.getSourceManager(),
                ci.getLangOpts());
        return true;
    }

    //
    // create AST consumer for this action
    //
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &ci, StringRef file) {
        llvm::outs() << ":: Working on " << file.str() << "... ";

        return std::unique_ptr<ASTConsumer>(new RewriterExampleASTConsumer(m_rewriter));
    }

    //
    // callback at the end of processing a single input
    //
    virtual void EndSourceFileAction() {
        //bool retVal = m_rewriter.overwriteChangedFiles();
        //if (retVal) {
        //    llvm::outs() << "failed\n";
        //}
        //else {
        //    llvm::outs() << "ok\n";
        //}

        llvm::outs() << "done\n";

        SourceManager &sm = m_rewriter.getSourceMgr();
        m_rewriter.getEditBuffer(sm.getMainFileID()).write(llvm::outs());

        llvm::outs() << "\n\n";
    }

private:
    Rewriter m_rewriter;
};


static llvm::cl::OptionCategory rewriterExampleTool("");

int main(int argc, const char **argv) {
    // parse command line arguments
    CommonOptionsParser options(argc, argv, rewriterExampleTool);

    // create clang tool instance
    ClangTool tool(
            options.getCompilations(),
            options.getSourcePathList());

    // run tool
    int retVal = tool.run(
            newFrontendActionFactory<RewriterExampleFrontendAction>().get());

    return retVal;
}
