add_library(Sema STATIC Pass.h
                        Sema.h
                        TypesPass.h TypesPass.cc
                        CFGPass.h CFGPass.cc
                        TerminatorPass.h TerminatorPass.cc)
