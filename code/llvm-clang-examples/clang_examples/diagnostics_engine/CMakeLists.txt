set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangAST clangBasic)


add_clang_executable(cl-demo_diagnostics_engine main.cc)
target_link_libraries(cl-demo_diagnostics_engine
        clangTooling
        clangASTMatchers
        clangBasic)

