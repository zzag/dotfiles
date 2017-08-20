LLVM & Clang code samples
=====================

LLVM & Clang 3.7 code samples


Build
=====
You need to have CMake and Ninja installed.

```
$ mkdir clang-llvm
$ cd clang-llvm
$ git clone http://llvm.org/git/llvm.git
$ cd llvm/tools
$ git clone http://llvm.org/git/clang.git
$ cd clang/tools
$ git clone https://github.com/vladzzag/llvm-clang-examples.git
$ echo "add_subdirectory(llvm-clang-examples)" >> CMakeLists.txt
$ vim CMakeLists # check llvm/tools/clang/tools/CMakeLists.txt
$ cd .. # go to clang-llvm/llvm/tools/clang
$ cd .. # go to clang-llvm/llvm/tools
$ cd .. # go to clang-llvm/llvm
$ cd .. # go to clang-llvm
$ mkdir build
$ cd build
$ cmake -G Ninja ../llvm -DLLVM_BUILD_TESTS=ON
$ ninja
```


Clang examples
==============

- Diagnostics engine;
- Traverse AST to count number of functions;
- Usage of clang::SyntaxOnlyAction;
- Rewriter;
- Handling attributes(GNU style \_\_attribute\_\_);
- Clang plugin;
- AST Matchers.


LLVM examples
=============

- Create @main
