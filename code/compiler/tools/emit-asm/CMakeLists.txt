add_executable(emit-asm main.cc)
llvm_map_components_to_libnames(llvm_libs
  Core
  Support)
target_link_libraries(emit-asm ${llvm_libs} Frontend)
