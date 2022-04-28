cmake_minimum_required(VERSION 3.12)
project(gobb_analyzer CXX)
include(CheckIncludeFileCXX)
include(GoogleTest)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

option(ENABLE_TESTING "Enable test" OFF)

#
# fmtlib.
#
add_subdirectory(fmt)

#
# gobb_analyze command.
#
add_executable(gobb_analyze
    analysis_cout_logger.cpp
    analysis_data_file_handler.cpp
    analyzer.cpp
    definitions.cpp
    position.cpp
    location_quad_maps.cpp
    piece_quad_index_maps.cpp
    transformer.cpp
    gobb_analyze.cpp)

set_target_properties(gobb_analyze PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON)
target_include_directories(gobb_analyze PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_options(gobb_analyze PUBLIC -Wall
    $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0> $<$<CONFIG:DEBUG>:-g3>)
target_link_options(gobb_analyze PUBLIC $<$<CONFIG:DEBUG>:-g3>)
target_link_libraries(gobb_analyze fmt::fmt-header-only)

#
# gobb_inspect command.
#
add_executable(gobb_inspect
    analysis_data_file_handler.cpp
    analyzer.cpp
    definitions.cpp
    inspector.cpp
    position.cpp
    position_text_creator.cpp
    location_quad_maps.cpp
    piece_quad_index_maps.cpp
    transformer.cpp
    gobb_inspect_processor.cpp
    gobb_inspect.cpp)

set_target_properties(gobb_inspect PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON)
target_include_directories(gobb_inspect PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_options(gobb_inspect PUBLIC -Wall
    $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0> $<$<CONFIG:DEBUG>:-g3>)
target_link_options(gobb_inspect PUBLIC $<$<CONFIG:DEBUG>:-g3>)
target_link_libraries(gobb_inspect fmt::fmt-header-only)

#
# gobb_test test program.
#
if(ENABLE_TESTING)
    find_package(GTest REQUIRED)
    add_executable(gobb_test
        definitions.cpp
        location_quad_maps.cpp
        piece_quad_index_maps.cpp
        position.cpp
        transformer.cpp
        position_test.cpp)

    set_target_properties(gobb_test PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON)
    target_include_directories(gobb_test PRIVATE ${PROJECT_SOURCE_DIR})
    target_compile_options(gobb_test PUBLIC -Wall
        $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0> $<$<CONFIG:DEBUG>:-g3>)
    target_link_libraries(gobb_test GTest::GTest GTest::Main)
    gtest_discover_tests(gobb_test)
endif()

#
# Installation.
#
install(TARGETS gobb_analyze gobb_inspect RUNTIME)

#
# Check header files.
#
check_include_file_cxx(unistd.h HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
    add_definitions(-DHAVE_UNISTD_H)
endif()

#
# Generate source files.
#
add_custom_command(
    OUTPUT location_quad_maps.cpp
    COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/generate_location_quad_maps.py
        > ${CMAKE_BINARY_DIR}/location_quad_maps.cpp
    DEPENDS generate_location_quad_maps.py
    COMMENT "Generates 'location_quad_maps.cpp'")

add_custom_command(
    OUTPUT piece_quad_index_maps.cpp
    COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/generate_piece_quad_index_maps.py
        > ${CMAKE_BINARY_DIR}/piece_quad_index_maps.cpp
    DEPENDS generate_piece_quad_index_maps.py
    COMMENT "Generates 'piece_quad_index_maps.cpp'")
