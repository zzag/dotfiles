
add_library(Codegen STATIC SymbolTable.h
                           CodegenContext.h CodegenContext.cc
                           Codegen.h Codegen.cc
                           ExprCodegen.h ExprCodegen.cc
                           StmtCodegen.h StmtCodegen.cc
                           DeclCodegen.h DeclCodegen.cc)
llvm_map_components_to_libnames(llvm_libs core native support)
target_link_libraries(Codegen ${llvm_libs})
