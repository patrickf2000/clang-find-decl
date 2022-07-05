cmake_minimum_required(VERSION 3.0.0)
project(find-decls)

find_package(LLVM REQUIRED CONFIG)
add_compile_options(-std=c++17 -g)
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})

find_package(Clang REQUIRED VERSION)

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
  
##
## Build the Rose tool
##
find_package(Boost
  COMPONENTS chrono date_time filesystem iostreams program_options random regex system wave thread
  REQUIRED)
  
include_directories(
    /home/patrick/llnl/cmake/install/include
    /home/patrick/llnl/cmake/install/include/rose
)
link_directories(/home/patrick/llnl/cmake/install/lib)
  
add_executable(rose-find-decls RoseFindDecls.cpp)

target_link_libraries(rose-find-decls
    -lrose
    ${Boost_LIBRARIES}
)

