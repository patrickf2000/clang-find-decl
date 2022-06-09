cmake_minimum_required(VERSION 3.0.0)
project(find-decls)

find_package(LLVM REQUIRED CONFIG)
add_compile_options(-std=c++17 -g)
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})

find_package(Clang REQUIRED)

set(LLVM_LINK_COMPONENTS
  Support
  )

add_executable(find-decls FindDecls.cpp)

target_link_libraries(find-decls
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )

