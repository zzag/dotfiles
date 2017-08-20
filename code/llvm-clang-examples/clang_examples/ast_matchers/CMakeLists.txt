set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangAST clangBasic)


add_clang_executable(cl-demo_ast_matchers main.cc)
target_link_libraries(cl-demo_ast_matchers
    clangTooling
    clangASTMatchers
    clangBasic)
