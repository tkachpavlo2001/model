cmake_minimum_required(VERSION 3.5)

project(acs_model_for_experiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    Controlled_process SHARED
    automated_control_system_element_interface.h
    automated_control_system_element_interface.cpp
    controlled_process.h
    controlled_process.cpp
)

target_include_directories(
    Controlled_process PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable( acs_model_for_experiments main.cpp DC_engine_tester.h)

target_link_libraries( acs_model_for_experiments PUBLIC Controlled_process )

install(TARGETS acs_model_for_experiments
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
