cmake_minimum_required(VERSION 3.5)

project(acs_model_for_experiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(GSL REQUIRED)
if(NOT GSL_FOUND)
    message(SEND_ERROR "Failed to find GSL")
else()
    include_directories(${GSL_INCLUDE_DIR})
endif()


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
)

add_library(
    DC_source SHARED
    dc_source.h dc_source.cpp
)

add_library(
    Regulator_tunner SHARED
    regulator_tuner.h
    regulator_tuner.cpp
)


#target_include_directories(
#    Controlled_process PUBLIC
#    ${CMAKE_CURRENT_SOURCE_DIR}
#s)

add_executable( acs_model_for_experiments
    main.cpp
    automated_control_system.h
    automated_control_system.cpp
    registrator.h
    registrator.cpp
    experiment_executor.h
    experiment_executor.cpp
    default_configuration_setter.h
    default_configuration_setter.cpp
)

add_executable( unit_test
    unit_test.cpp
    automated_control_system.h
    automated_control_system.cpp
    registrator.h
    registrator.cpp
    experiment_executor.h
    experiment_executor.cpp
    default_configuration_setter.cpp
    regulator_tuner.cpp
)

target_link_libraries( acs_model_for_experiments PUBLIC Controlled_process )
target_link_libraries( acs_model_for_experiments PUBLIC Regulator)
target_link_libraries( acs_model_for_experiments PUBLIC DC_source)
target_link_libraries( acs_model_for_experiments PUBLIC Regulator_tunner)
target_link_libraries( acs_model_for_experiments PUBLIC ${GSL_LIBRARY})

target_link_libraries( unit_test PRIVATE
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${GSL_LIBRARY}
    Controlled_process
    Regulator
    DC_source
    Regulator_tunner
)

#target_link_libraries(acs_model_for_experiments GSL::gsl GSL::gslcblas)
#target_link_libraries(unit_test GSL::gsl GSL::gslcblas)


#install(TARGETS acs_model_for_experiments
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
