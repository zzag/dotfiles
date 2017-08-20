#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include <memory>
#include <vector>
#include <string>


using namespace clang;
using namespace clang::tooling;
using namespace llvm;
using std::unique_ptr;
using std::vector;
using std::string;


/**
 *
 * RUN:
 * 
 * First way:
 * $ ninja
 * $ bin/clang++ -cc1 -load lib/cl-demo_plugin.so -plugin demo-plugin \
 *      -plugin-arg-demo-plugin -print-hello -c some_file.cc
 *
 * Second way(use clang as usual):
 * $ ninja
 * $ bin/clang++ -Xclang -load -Xclang lib/cl-demo_plugin.so \
 *      -Xclang -plugin -Xclang demo-plugin \
 *      -Xclang -plugin-arg-demo-plugin -Xclang -print-hello some_file.cc
 *
 * SYNTAX:
 * clang -load libPlugin.so [-plugin plugin-name [-plugin-arg-plugin-name -arg]] file.c
 * 
 */


class DemoPluginConsumer : public ASTConsumer {
	//
	// some code here
	//
};


class DemoPluginAction : public PluginASTAction {
public:
	unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef InFile) {
		return unique_ptr<ASTConsumer>(new DemoPluginConsumer);
	}

	virtual bool ParseArgs(const CompilerInstance &CI, const vector<string> &args) {
		for (auto arg : args) {
			if (arg == "-print-hello") {
				llvm::outs() << "Hello, world!" << "\n";
			}
		}

		return true;
	}
};


static FrontendPluginRegistry::Add<DemoPluginAction> X("demo-plugin", "Demo plugin");