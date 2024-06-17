cmake_minimum_required(VERSION 3.5)

project(acs_model_for_experiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(acs_model_for_experiments main.cpp)

install(TARGETS acs_model_for_experiments
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
