#include "frontend/CompilerFrontend.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/CodeGen/CommandFlags.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support//Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/LegacyPassManager.h"
#include <memory>


using namespace llvm;


// command line options
static cl::opt<std::string>
InputFilename(cl::Positional, cl::desc("<input bitcode>"));

static cl::opt<std::string>
OutputFilename("o", cl::desc("Output filename"), cl::value_desc("filename"));

static cl::opt<std::string>
TargetTriple("mtriple", cl::desc("Override target triple for module"));

static cl::opt<char>
OptLevel("O",
         cl::desc("Optimization level. [-O0, -O1, -O2, or -O3] "
                  "(default = '-O2')"),
         cl::Prefix,
         cl::ZeroOrMore,
         cl::init(' '));

static cl::opt<bool>
NoIntegratedAssembler("no-integrated-as", cl::Hidden,
                      cl::desc("Disable integrated assembler"));

static cl::opt<bool> ShowMCEncoding("show-mc-encoding", cl::Hidden,
                                    cl::desc("Show encoding in .s output"));

static cl::opt<bool> EnableDwarfDirectory(
    "enable-dwarf-directory", cl::Hidden,
    cl::desc("Use .file directives with an explicit directory."));

static cl::opt<bool> AsmVerbose("asm-verbose",
                                cl::desc("Add comments to directives."),
                                cl::init(true));


int compileModule(Module* module);

int main(int argc, char** argv)
{
  sys::PrintStackTraceOnErrorSignal();
  PrettyStackTraceProgram X(argc, argv);

  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  InitializeNativeTargetAsmParser();

  cl::AddExtraVersionPrinter(TargetRegistry::printRegisteredTargetsForVersion);
  cl::ParseCommandLineOptions(argc, argv, "object file emitter\n");

  CompilerFrontend frontend(InputFilename);
  Module* module = frontend.codegen();
  if (! module) return -1;

  return compileModule(module);
}


const Target*
getTarget(Triple triple)
{
  std::string error;
  const Target* target = TargetRegistry::lookupTarget(MArch, triple, error);
  if (! target)
    errs() << error;

  return target;
}


std::unique_ptr<tool_output_file>
getOutputStream()
{
  if (OutputFilename.empty()) {
    StringRef ifn = InputFilename;

    if (ifn.endswith(".txt"))
      OutputFilename =  (ifn.drop_back(4) + ".s").str();
    else
      OutputFilename = (ifn + ".s").str();
  }

  std::error_code ec;
  auto out = llvm::make_unique<tool_output_file>(OutputFilename, ec, sys::fs::F_Text);
  if (ec) {
    errs() << ec.message() << '\n';
    return nullptr;
  }

  return out;
}


int compileModule(Module* module)
{
  Triple triple;

  if (! TargetTriple.empty())
    module->setTargetTriple(Triple::normalize(TargetTriple));
  triple = Triple(module->getTargetTriple());

  if (triple.getTriple().empty())
    triple.setTriple(sys::getDefaultTargetTriple());

  const Target* theTarget = getTarget(triple);
  if (! theTarget) return -1;

  std::string CPUStr = getCPUStr();
  std::string featuresStr = getFeaturesStr();

  CodeGenOpt::Level OLvl = CodeGenOpt::Default;
  switch (OptLevel) {
  default:
    errs() << ": invalid optimization level.\n";
    return -1;
  case ' ': break;
  case '0': OLvl = CodeGenOpt::None; break;
  case '1': OLvl = CodeGenOpt::Less; break;
  case '2': OLvl = CodeGenOpt::Default; break;
  case '3': OLvl = CodeGenOpt::Aggressive; break;
  }

  TargetOptions options = InitTargetOptionsFromCodeGenFlags();
  options.DisableIntegratedAS = NoIntegratedAssembler;
  options.MCOptions.ShowMCEncoding = ShowMCEncoding;
  options.MCOptions.MCUseDwarfDirectory = EnableDwarfDirectory;
  options.MCOptions.AsmVerbose = AsmVerbose;

  std::unique_ptr<TargetMachine> target(
      theTarget->createTargetMachine(triple.getTriple(), CPUStr, featuresStr,
                                     options, RelocModel, CMModel, OLvl));

  if (! target) return -1;

  if (FloatABIForCalls != FloatABI::Default)
    options.FloatABIType = FloatABIForCalls;

  std::unique_ptr<tool_output_file> out = getOutputStream();
  if (! out) return -1;

  legacy::PassManager PM;

  TargetLibraryInfoImpl TLII(Triple(module->getTargetTriple()));
  PM.add(new TargetLibraryInfoWrapperPass(TLII));

  module->setDataLayout(target->createDataLayout());

  setFunctionAttributes(CPUStr, featuresStr, *module);

  raw_pwrite_stream *os = &out->os();

  if (target->addPassesToEmitFile(PM, *os, TargetMachine::CGFT_AssemblyFile)) {
    errs() << " target does not support generation of this file type!\n";
    return -1;
  }

  PM.run(*module);

  out->keep();
  return 0;
}
