cmake_minimum_required(VERSION 3.5)

project(acs_model_for_experiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Charts)
find_package(Qt6 REQUIRED COMPONENTS Core)


if(NOT Qt6_FOUND)
    message(SEND_ERROR "Failed to find Qt6")
endif()


include_directories(${Qt6Charts_INCLUDE_DIRS})

add_executable( ACS_MODEL WIN32
    main.cpp
    automated_control_system.h
    automated_control_system.cpp
    default_configuration_setter.cpp
    registrator.h
    registrator.cpp
    experiment_executor.h
    experiment_executor.cpp
    default_configuration_setter.h
    default_configuration_setter.cpp
    tkachpavlo2001lib/tkachpavlo2001lib.hpp
    automated_control_system_element_interface.cpp
    automated_control_system.cpp
    reference_signal_definder_static.cpp
    dc_source.cpp
    controlled_process.cpp
    automated_control_system_element_interface.cpp
    controlled_process.cpp
    controlled_process.h
    default_configuration_setter.cpp
    automated_control_system_element_interface.h
    automated_control_system_element_interface.cpp
    regulator.cpp
    regulator.h
    reference_signal_definder_static.h
    reference_signal_definder_static.cpp
    generative_algorithm.h
    default_configuration_setter.cpp
    dc_source.h
    dc_source.cpp
    automated_control_system_element_interface.cpp
    default_configuration_setter.cpp
    regulator_tuner.h
    regulator_tuner.cpp
    generative_algorithm.h
    gradient_method.h
    fitness_function.cpp
    container_analyzer.h
    container_analyzer.cpp
    automated_control_system_element_interface.cpp
    default_configuration_setter.cpp
    mainwindow.h
    mainwindow.cpp
    timerapp.h
    timerapp.cpp
    chartwidgetfactory.h
    chartwidgetfactory.cpp
)

target_link_libraries( ACS_MODEL PRIVATE Qt6::Widgets)
target_link_libraries( ACS_MODEL PRIVATE Qt6::Charts)
