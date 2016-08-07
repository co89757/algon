cmake_minimum_required(VERSION 2.8)
project(algon)
aux_source_directory(${PROJECT_SOURCE_DIR} SRC)
MESSAGE(STATUS "SRC = " ${SRC})
aux_source_directory( ${PROJECT_SOURCE_DIR}/dstruct DS_SRC)
aux_source_directory( ${PROJECT_SOURCE_DIR}/utility UTIL_SRC)
aux_source_directory( ${PROJECT_SOURCE_DIR}/algorithm ALGO_SRC)
list(APPEND SRC ${DS_SRC} ${UTIL_SRC} ${ALGO_SRC} )
add_compile_options(-std=c++11)
SET(CXX_STANDARD 11)

MESSAGE("Compile flags options: " ${CMAKE_CXX_FLAGS} )
MESSAGE("Sources: " ${SRC} )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_executable(sample ${SRC})
set_property(TARGET sample APPEND_STRING
  PROPERTY COMPILE_FLAGS "-Wall")