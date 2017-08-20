set(SOURCE_FILES ForceBasedSimulator.cc)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(HEADER_FILES ${INCLUDE_DIR}/Graph.h
                 ${INCLUDE_DIR}/ForceBasedSimulator.h)


add_library(ForceBasedRender SHARED ${SOURCE_FILES} ${HEADER_FILES})