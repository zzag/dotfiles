# LLVM arguments


```cpp
Type *args[] = { builder.getInt8Ty()->getPointerTo() };
FunctionType *funcType = FunctionType::get(builder.getInt32Ty(), args, false);
Function *coolFunc = Function::Create(funcType, Function::ExternalLinkage, "coolFn", module);
BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", coolFunc);
builder.SetInsertBlock(BB);

AttrBuilder attrBuilder;
attrBuilder.clear();
attrBuilder.addAttribute(Attribute::NoAlias);
attrBuilder.addAttribute(Attribute::NoCapture);

Function::arg_iterator argsIterator = coolFunc->arg_begin();
Argument *a = argsIterator++;
a->addAttr(AttributeSet::get(getGlobalContext(), 0, attrBuilder));
```
