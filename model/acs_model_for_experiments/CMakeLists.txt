cmake_minimum_required(VERSION 3.5)

project(acs_model_for_experiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_library(
    Controlled_process SHARED
    DC_engine_tester.h
    automated_control_system_element_interface.h
    automated_control_system_element_interface.cpp
    controlled_process.h
    controlled_process.cpp
)

#target_include_directories(
#    Controlled_process PUBLIC
#    ${CMAKE_CURRENT_SOURCE_DIR}
#s)

add_executable( acs_model_for_experiments main.cpp)

add_executable( unit_test unit_test.cpp)

target_link_libraries( acs_model_for_experiments PUBLIC Controlled_process )

target_link_libraries( unit_test PRIVATE
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    Controlled_process )

#install(TARGETS acs_model_for_experiments
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
