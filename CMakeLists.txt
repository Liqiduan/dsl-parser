cmake_minimum_required(VERSION 2.8.2)
project(dsl-parser)

find_package(cmocka REQUIRED)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(parser src/dsl.y ${CMAKE_CURRENT_BINARY_DIR}/dsl.y.c)
flex_target(scanner src/dsl.l ${CMAKE_CURRENT_BINARY_DIR}/dsl.l.c)
add_flex_bison_dependency(scanner parser)

add_library(dsl-parser SHARED 
        src/dsl.c
        ${BISON_parser_OUTPUTS}
        ${FLEX_parser_OUTPUTS})
include_directories(dsl-parser
        PUBLIC fl
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(dsl-parser-test src/dsl_test.c)
target_link_libraries(dsl-parser-test 
        PRIVATE dsl-parser)
