find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(_parser internal/Parser.y ${CMAKE_CURRENT_BINARY_DIR}/_Parser.cc)
flex_target(_scanner internal/Scanner.l ${CMAKE_CURRENT_BINARY_DIR}/_Scanner.cc)
add_flex_bison_dependency(_scanner _parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(Parser STATIC Parser.cc
                          AST.cc
                          ${BISON__parser_OUTPUTS}
                          ${FLEX__scanner_OUTPUTS})
