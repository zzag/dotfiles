#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include <memory>


using namespace clang;
using namespace clang::tooling;
using namespace llvm;

const char endl = '\n';


/**
 *
 * Execute:
 *  $ cl-demo_attributes example.cc --
 *
 */


class AttributesASTVisitor : public RecursiveASTVisitor<AttributesASTVisitor> {
public:
	bool VisitFunctionDecl(FunctionDecl *decl) {
		// print function name
		llvm::outs() << decl->getName() << "() attributes: " << endl;

		for (auto *attr : decl->attrs()) {
			HandleAttr(attr);
		}

		llvm::outs() << endl << endl; 
		
		return true;
	}

	bool VisitCXXRecordDecl(CXXRecordDecl *decl) {
		// print name
		if (decl->isStruct()) { llvm::outs() << "struct "; }
		else if (decl->isClass()) { llvm::outs() << "class "; }
		else if (decl->isUnion()) { llvm::outs() << "union "; }
		else if (decl->isEnum()) { llvm::outs() << "enum "; }

		llvm::outs() << decl->getName() << " attributes: " << endl;

		for (auto *attr : decl->attrs()) {
			HandleAttr(attr);
		}

		llvm::outs() << endl << endl; 
		
		return true;
	}

	void HandleAttr(Attr *attr) {
		//
		// take a look at clang::attr::Kind
		//
		//
		if (isa<AnnotateAttr>(attr)) {
			AnnotateAttr *annotate = dyn_cast<AnnotateAttr>(attr);
			llvm::outs() << "    annotate(" << annotate->getAnnotation() << ")" << endl;
		}
		else if (isa<DeprecatedAttr>(attr)) {
			llvm::outs() << "    deprecated" << endl;
		}
		else {
			llvm::outs() << "    Unhandled attribute!" << endl;
		}
	}
};


class AttributesASTConsumer : public ASTConsumer {
public:
    virtual void HandleTranslationUnit(ASTContext &context) {
        m_visitor.TraverseDecl(context.getTranslationUnitDecl());
    }

private:
	AttributesASTVisitor m_visitor;
};


class AttributesFrontendAction : public ASTFrontendAction {
public:
    //
    // create AST consumer for this action
    //
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &ci, StringRef file) {
        return std::unique_ptr<ASTConsumer>(new AttributesASTConsumer);
    }
};


static llvm::cl::OptionCategory attributesTool("");

int main(int argc, const char **argv) {
    // parse command line arguments
    CommonOptionsParser options(argc, argv, attributesTool);

    // create clang tool instance
    ClangTool tool(options.getCompilations(), options.getSourcePathList());

    // run tool
    int retVal = tool.run(newFrontendActionFactory<AttributesFrontendAction>().get());

    return retVal;
}
