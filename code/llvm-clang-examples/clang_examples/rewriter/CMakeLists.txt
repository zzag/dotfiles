set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangAST clangBasic)


add_clang_executable(cl-demo_rewriter main.cc)
target_link_libraries(cl-demo_rewriter
    clangTooling
    clangASTMatchers
    clangBasic)
