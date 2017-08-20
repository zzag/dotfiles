#include "Codegen.h"
#include "llvm/IR/LLVMContext.h"


using namespace llvm;


llvm::Type*
Codegen::getType(std::string type)
{
  if (type == "i32") return reinterpret_cast<Type*>(Type::getInt32Ty(getGlobalContext()));
  else if (type == "bool") return reinterpret_cast<Type*>(Type::getInt1Ty(getGlobalContext()));
  else if (type == "f32") return Type::getFloatTy(getGlobalContext());
  else if (type == "str") return reinterpret_cast<Type*>(reinterpret_cast<Type*>(Type::getInt8Ty(getGlobalContext()))->getPointerTo());
  return Type::getVoidTy(getGlobalContext());
}
