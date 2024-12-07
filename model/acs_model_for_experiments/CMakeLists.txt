cmake_minimum_required(VERSION 3.5)

project(acs_model_for_experiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(GSL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Charts)
find_package(Qt6 REQUIRED COMPONENTS Core)


if(NOT Qt6_FOUND)
    message(SEND_ERROR "Failed to find Qt6")
endif()

if(NOT GSL_FOUND)
    message(SEND_ERROR "Failed to find GSL")
else()
    include_directories(${GSL_INCLUDE_DIR})
endif()
include_directories(tkachpavlo2001lib)

include_directories(${Qt6Charts_INCLUDE_DIRS})



#qt_wrap_ui(UI_HEADERS dialog.ui)
add_library( Interactive_part SHARED
    mainwindow.h
    mainwindow.cpp
    timerapp.h
    timerapp.cpp
    chartwidgetfactory.h
    chartwidgetfactory.cpp
)
target_link_libraries( Interactive_part PRIVATE Qt6::Widgets Qt6::Charts Qt6::Core)

add_library( Service_units SHARED
    registrator.h
    registrator.cpp
    experiment_executor.h
    experiment_executor.cpp
    default_configuration_setter.h
    default_configuration_setter.cpp
    tkachpavlo2001lib/tkachpavlo2001lib.hpp
)
target_link_libraries( Service_units PRIVATE Qt6::Widgets Qt6::Charts )

add_library(
    Controlled_process SHARED
    automated_control_system_element_interface.cpp
    controlled_process.cpp
    controlled_process.h
)

add_library(
    Regulator SHARED
    automated_control_system_element_interface.h
    automated_control_system_element_interface.cpp
    regulator.cpp
    regulator.h
    reference_signal_definder_static.h
    reference_signal_definder_static.cpp
    generative_algorithm.h
)

add_library(
    DC_source SHARED
    dc_source.h dc_source.cpp
)

add_library(
    Regulator_tunner SHARED
    regulator_tuner.h
    regulator_tuner.cpp
    generative_algorithm.h
    gradient_method.h
    fitness_function.cpp
    container_analyzer.h
    container_analyzer.cpp
)

add_executable( acs_model_for_experiments
    main.cpp
    automated_control_system.h
    automated_control_system.cpp
)

#add_executable( unit_test
#    unit_test.cpp
#    automated_control_system.h
#    automated_control_system.cpp
#    registrator.h
#    registrator.cpp
#    experiment_executor.h
#    experiment_executor.cpp
#    default_configuration_setter.cpp
#    regulator_tuner.cpp
#)

target_link_libraries( acs_model_for_experiments PUBLIC Controlled_process )
target_link_libraries( acs_model_for_experiments PUBLIC Regulator)
target_link_libraries( acs_model_for_experiments PUBLIC DC_source)
target_link_libraries( acs_model_for_experiments PUBLIC Regulator_tunner)
target_link_libraries( acs_model_for_experiments PUBLIC Interactive_part)
target_link_libraries( acs_model_for_experiments PUBLIC Service_units)
target_link_libraries( acs_model_for_experiments PRIVATE Qt6::Widgets)
target_link_libraries( acs_model_for_experiments PRIVATE Qt6::Charts)
target_link_libraries( acs_model_for_experiments PUBLIC ${GSL_LIBRARY})

#target_link_libraries( unit_test PRIVATE
#    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
#    ${GSL_LIBRARY}
#    Controlled_process
#    Regulator
#    DC_source
#    Regulator_tunner
#)
