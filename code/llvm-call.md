# LLVM CreateCall


```cpp
Type *args[] = { builder.getInt8Ty()->getPointerTo() };
FunctionType *putsType = FunctionType::get(builder.getInt32Ty(), args, false);
Constant *putsFunc = module->getOrInsertFunction("puts", putsType);
```

```cpp
Value *args[] = { builder.CreateGlobalStringPtr("random stuff") };
builder.CreateCall(putsFunc, args);
```
