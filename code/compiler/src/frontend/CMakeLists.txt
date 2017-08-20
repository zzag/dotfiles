add_library(Frontend STATIC CompilerFrontend.h CompilerFrontend.cc)
target_link_libraries(Frontend Parser Sema Codegen)
