add_executable(emit-elf main.cc)
llvm_map_components_to_libnames(llvm_libs
  Core
  Support)
target_link_libraries(emit-elf ${llvm_libs} Frontend)
