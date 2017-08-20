#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include <memory>
#include <string>
#include <cctype>

/**
 *
 * Note:
 *  * DiagnosticsEngine::Level::Ignored
 *  * DiagnosticsEngine::Level::Note
 *
 * is not visible.
 * Use: Remark, Warning, Error, Fatal.
 *
 *
 * Usage:
 * $ cl-demo_diagnostics_engine example.cc --
 *
 */


using namespace clang;
using namespace clang::tooling;
using namespace llvm;


class DiagnosticsEngineExampleVisitor : 
    public RecursiveASTVisitor<DiagnosticsEngineExampleVisitor> {
public:
    DiagnosticsEngineExampleVisitor () {
        m_context = nullptr;
    }

    virtual ~DiagnosticsEngineExampleVisitor() {}

    // 2nd way
    void setContext(ASTContext *context) {
        m_context = context;
    }

    // visit class, struct, enum, union
    virtual bool VisitCXXRecordDecl(CXXRecordDecl *cxxRecordDecl) {
        // print something
        DiagnosticsEngine &diagnosticsEngine = m_context->getDiagnostics();
        
        // first get dialog ID
        unsigned dialogID = diagnosticsEngine.getCustomDiagID(
                DiagnosticsEngine::Level::Remark,
                "Dude, I\'m visiting CXX Record!!!");
        
        // get location of the record 
        SourceLocation location = cxxRecordDecl->getLocation();

        // report it
        diagnosticsEngine.Report(location, dialogID);


        if (cxxRecordDecl->isStruct()) {
            // it's struct
            return this->visitStructDecl(cxxRecordDecl);
        }
        else if (cxxRecordDecl->isClass()) {
            // it's class
            return this->visitClassDecl(cxxRecordDecl);
        }
        else if (cxxRecordDecl->isUnion()) {
            // it's union
            return this->visitUnionDecl(cxxRecordDecl);
        }
        else if (cxxRecordDecl->isEnum()) {
            // it's enum
            return this->visitEnumDecl(cxxRecordDecl);
        }
        else {
            // WTF dude?
            unsigned wtfDialogID = diagnosticsEngine.getCustomDiagID(
                    DiagnosticsEngine::Level::Error,
                    "WTF dude?");

            // report problems
            diagnosticsEngine.Report(location, wtfDialogID);
        }

        return true;
    }

    // Visit class
    virtual bool visitClassDecl(CXXRecordDecl *decl) {
        // get class name
        StringRef className = decl->getName();

        // is it lower case?
        if (islower(static_cast<int>(className[0]))) {
            // prepare message
            DiagnosticsEngine &diagnosticsEngine = m_context->getDiagnostics();
            unsigned dialogID = diagnosticsEngine.getCustomDiagID(
                    DiagnosticsEngine::Level::Warning,
                    "Class name should be capitalized.");
            SourceLocation classNameStart = decl->getLocation();
            SourceLocation classNameEnd = 
                classNameStart.getLocWithOffset(className.size());

            // prepare fix it hint
            std::string tempReplacement = className;
            tempReplacement[0] = toupper(tempReplacement[0]);
            tempReplacement = "Should be - \'" + tempReplacement + "\'!";
            StringRef replacement(tempReplacement);

            FixItHint fixIt = FixItHint::CreateReplacement(
                    SourceRange(classNameStart, classNameEnd),
                    replacement);

            // print it
            // NOTE: Report() returns DiagnosticBuilder:
            //  -> void AddFixItHint(const FixItHint &)
            //  -> void AddString(StringRef)
            diagnosticsEngine.Report(classNameStart, dialogID).
                              AddFixItHint(fixIt);
        }

        return true;
    }

    // Visit struct
    virtual bool visitStructDecl(CXXRecordDecl *decl) {
        // ...
        return true;
    }

    // Visit union
    virtual bool visitUnionDecl(CXXRecordDecl *decl) {
        // ...
        return true;
    }

    // visit enum
    // Also, you can use VisitEnumDecl
    virtual bool visitEnumDecl(CXXRecordDecl *decl) {
        // ...
        return true;
    }
    
    // check Function name
    virtual bool VisitFunctionDecl(FunctionDecl *functionDecl) {
        // get function name
        std::string functionName = functionDecl->getName().str();
        
        // is it magicFunction?
        if (functionName == "magicFunction") {
            // prepare message
            DiagnosticsEngine &diagnosticsEngine = m_context->getDiagnostics();
            unsigned dialogID = diagnosticsEngine.getCustomDiagID(
                    DiagnosticsEngine::Level::Fatal,
                    "You can\'t use function with name \"magicNumber\"");
            SourceLocation location = functionDecl->getLocation();

            // print
            diagnosticsEngine.Report(location, dialogID);
        }

        return true;
    }

private:
    ASTContext *m_context;
};


class DiagnosticsEngineExampleConsumer : public ASTConsumer {
public:
    virtual void HandleTranslationUnit(ASTContext &context) {
        m_visitor.setContext(&context);
        m_visitor.TraverseDecl(context.getTranslationUnitDecl());
    }

private:
    DiagnosticsEngineExampleVisitor m_visitor;
};


class DiagnosticsEngineExampleFrontendAction : public ASTFrontendAction {
public:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &ci, StringRef file) {
        return std::unique_ptr<ASTConsumer>(new DiagnosticsEngineExampleConsumer);
    }
};


static llvm::cl::OptionCategory diagnosticsEngineCategory("");

int main(int argc, const char **argv) {
    // parse command line arguments
    CommonOptionsParser options(argc, argv, diagnosticsEngineCategory);

    // create clang tool instance
    ClangTool tool(
            options.getCompilations(),
            options.getSourcePathList());

    // run tool
    int retVal = tool.run(
            newFrontendActionFactory<DiagnosticsEngineExampleFrontendAction>().get());
 
    return retVal;
}
