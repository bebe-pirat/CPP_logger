# clang-format.cmake
file(GLOB_RECURSE ALL_SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

add_custom_target(
    clangformat
    COMMAND clang-format
    -style=LLVM
    -i
    ${ALL_SOURCE_FILES}
)