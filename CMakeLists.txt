cmake_minimum_required(VERSION 2.8.2)
project(dsl-parser)

set(CMAKE_CXX_FLAGS_DEBUG -g)

find_package(cmocka REQUIRED)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(parser src/dsl_parser.y ${CMAKE_CURRENT_BINARY_DIR}/dsl_parser.y.c)
flex_target(scanner src/dsl_parser.l ${CMAKE_CURRENT_BINARY_DIR}/dsl_parser.l.c)
add_flex_bison_dependency(scanner parser)

add_library(dsl-parser SHARED 
        src/dsl.c
        ${BISON_parser_OUTPUTS}
        ${FLEX_scanner_OUTPUTS})

target_include_directories(dsl-parser
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dsl-parser
    PUBLIC fl)

add_executable(dsl-parser-test test/dsl_test.c)
target_link_libraries(dsl-parser-test 
        PRIVATE dsl-parser cmocka)

add_executable(dsl src/dsl_parser.c)
target_link_libraries(dsl PRIVATE dsl-parser)

enable_testing()
add_test(dsl-parser-test dsl-parser-test)
