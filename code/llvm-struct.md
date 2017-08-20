# LLVM StructType


```cpp
StructType *structType = StructType::create(context, "Object");
Type *elements[] = { builder.getInt32Ty(), builder.getInt8Ty() };
structType.setBody(elements);
```


```cpp
Value *instance = builder.CreateAlloca(structType, 0);


// 1
Value *idx[] = { ConstantInt::get(context, APInt(32, 0)),
                 ConstantInt::get(context, APInt(32, 0)) };
Value *ptr = builder.CreateInBoundsGEP(instance, idx, "ptr");

// 2
unsigned idx = 0;
Value *ptr = builder.CreateStructGEP(structType, instance, idx, "ptr");
```

```cpp
builder.CreateStore(ConstantInt::get(context, APInt(32, 123)),
                    ptr);
```

```cpp
Value *val = builder.CreateLoad(ptr, "val");
```
