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

add_executable( acs_model_for_experiments main.cpp)

target_link_libraries( acs_model_for_experiments Controlled_process)

install(TARGETS acs_model_for_experiments
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
