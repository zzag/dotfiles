add_executable(emit-obj main.cc)
llvm_map_components_to_libnames(llvm_libs
  Core
  Support)
target_link_libraries(emit-obj ${llvm_libs} Frontend)
