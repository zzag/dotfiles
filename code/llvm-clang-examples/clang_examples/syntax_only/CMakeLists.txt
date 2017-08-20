set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangAST clangBasic)

# add executable
add_clang_executable(cl-demo_syntax_only main.cc)
target_link_libraries(cl-demo_syntax_only 
    clangTooling
    clangASTMatchers
    clangBasic)
