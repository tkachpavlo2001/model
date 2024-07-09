#define BOOST_TEST_MAIN

#define BOOST_TEST_DYN_LINK

#include<boost/test/unit_test.hpp>
#include"controlled_process.h"
#include<memory>
#include<algorithm>


BOOST_AUTO_TEST_SUITE(DC_engine_interface_testing)

BOOST_AUTO_TEST_CASE(case_1_1_DC_engine_object_creating)
{
    std::shared_ptr<DC_engine> p_drive = nullptr;
    p_drive = std::make_shared<DC_engine>();
    BOOST_REQUIRE(p_drive != nullptr);
}

BOOST_AUTO_TEST_CASE(case_1_2_Verifying_to_set_dt)
{
    DC_engine drive;
    drive.to_set_dt(2.2);
    BOOST_REQUIRE(drive.to_check_parameters()[DC_engine::DT] == 2.2);
}

BOOST_AUTO_TEST_CASE(case_1_3_Verifying_to_receive_input_signal)
{
    DC_engine drive;
    drive.to_receive_input_signal(3.3);
    BOOST_REQUIRE_EQUAL(drive.to_check_parameters()[DC_engine::INPUT_SIGNAL], 3.3);
}

BOOST_AUTO_TEST_CASE(case_1_4_Verifying_to_set_all_parameters)
{
    DC_engine drive;
    double num = 0;
    std::vector<double> parameters_to_set = drive.to_check_parameters();
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1 * num;
    drive.to_set_all_parameters(parameters_to_set);
    parameters_to_set[DC_engine::TORQUE_OF_LOAD] =
            parameters_to_set[DC_engine::LOAD_K_0] +
            parameters_to_set[DC_engine::LOAD_K_1] * parameters_to_set[DC_engine::VELOCITY] +
            parameters_to_set[DC_engine::LOAD_K_2] * parameters_to_set[DC_engine::VELOCITY] * parameters_to_set[DC_engine::VELOCITY];
    parameters_to_set[DC_engine::MOMENT_OF_INERTIA] =
            parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] +
            parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_ENGINE];
    BOOST_REQUIRE(
                std::equal(
                    drive.to_check_parameters().begin(),
                    drive.to_check_parameters().end(),
                    parameters_to_set.begin()
                    )
                );
    BOOST_WARN_MESSAGE( (drive.to_check_parameters()[DC_engine::SIZE] + 1) == DC_engine::SIZE, "Size-problem");
}

BOOST_AUTO_TEST_CASE(case_1_5_Verifying_to_check_parameters_and_to_get_parameters)
{
    DC_engine drive;
    double num = 1;
    std::vector<double> parameters_to_set = std::vector<double>(DC_engine::SIZE, 0);
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1.1 * num;
    drive.to_set_all_parameters(parameters_to_set);
    BOOST_REQUIRE(
                std::equal(
                    drive.to_check_parameters().begin(),
                    drive.to_check_parameters().end(),
                    drive.to_get_parameters().begin()
                    )
                );
}

BOOST_AUTO_TEST_CASE(case_1_6_Verifying_amount_of_parameters_methods)
{
    DC_engine drive;
    double num = 0;
    std::vector<double> parameters_to_set = drive.to_check_parameters();
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1 * num;
    drive.to_set_all_parameters(parameters_to_set);
    BOOST_WARN_MESSAGE(drive.to_check_amount_of_parameters() == DC_engine::SIZE, "to_check_amount_of_parameters() -- problem");
    BOOST_WARN_MESSAGE(drive.to_verify_amount_of_parameters(), "to_verify_amount_of_parameters() -- problem");
    BOOST_REQUIRE(
                drive.to_check_amount_of_parameters() == DC_engine::SIZE &&
                drive.to_verify_amount_of_parameters()
                );
}

BOOST_AUTO_TEST_CASE(case_1_7_Verifying_to_get_output_signal)
{
    DC_engine drive;
    std::vector<double> parameters_to_set = drive.to_check_parameters();
    parameters_to_set[DC_engine::OUTPUT_SIGNAL] = 4.4;
    drive.to_set_all_parameters(parameters_to_set);
    BOOST_REQUIRE_EQUAL(drive.to_get_output_signal(), parameters_to_set[DC_engine::OUTPUT_SIGNAL]);
    BOOST_WARN_MESSAGE(drive.to_check_parameters()[DC_engine::OUTPUT_SIGNAL] == parameters_to_set[DC_engine::OUTPUT_SIGNAL],
            "OUTPUT_SIGNAL is not the same as the planned one\n"
            "Maybe due to this case depends on case_4_Verifying_to_set_all_parameters\n"
            "Check the case_1_4_Verifying_to_set_all_parameters firstly"
            );
}

BOOST_AUTO_TEST_CASE(case_1_8_Verifying_to_set_parameters)
{
    DC_engine drive;
    double num = 0;
    std::vector<double> parameters_to_initiate = drive.to_check_parameters();
    std::vector<double> parameters_to_set = drive.to_check_parameters();
    for (auto i = std::begin(parameters_to_initiate); i < std::end(parameters_to_initiate); ++num, ++i)
        *i = 1 * num;
    drive.to_set_all_parameters(parameters_to_initiate);
    num = DC_engine::END_STATIC;
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1.1 * num;
    drive.to_set_parameters(parameters_to_set);
    parameters_to_set[DC_engine::TORQUE_OF_LOAD] =
            parameters_to_set[DC_engine::LOAD_K_0] +
            parameters_to_set[DC_engine::LOAD_K_1] * parameters_to_set[DC_engine::VELOCITY] +
            parameters_to_set[DC_engine::LOAD_K_2] * parameters_to_set[DC_engine::VELOCITY] * parameters_to_set[DC_engine::VELOCITY];
    parameters_to_set[DC_engine::MOMENT_OF_INERTIA] =
            parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] +
            parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_ENGINE];
    {
        auto i = std::begin(drive.to_check_parameters());
        auto j = std::begin(parameters_to_initiate);
        for(; i < std::begin(drive.to_check_parameters()) + DC_engine::BEGIN_NONSTATIC; ++i, ++j)
            std::cout << *i << " = " << *j << std::endl;
    }

    {
        auto i = std::begin(drive.to_check_parameters()) + DC_engine::BEGIN_NONSTATIC;
        auto j = std::begin(parameters_to_set);
        for(; i < std::end(drive.to_check_parameters()); ++i, ++j)
            std::cout << *i << " = " << *j << std::endl;
    }
    BOOST_WARN_MESSAGE(
                std::equal(
                    std::begin(drive.to_check_parameters()),
                    std::begin(drive.to_check_parameters()) + DC_engine::BEGIN_NONSTATIC,
                    std::begin(parameters_to_initiate)
                    ), "The static part has been impacted");
    BOOST_WARN_MESSAGE(
                std::equal(
                    std::begin(drive.to_check_parameters()) + DC_engine::BEGIN_NONSTATIC,
                    std::end(drive.to_check_parameters()),
                    std::begin(parameters_to_set)
                    ), "The nonstatic part has been incorrectly modificated");
    BOOST_REQUIRE(
                std::equal(
                    std::begin(drive.to_check_parameters()),
                    std::begin(drive.to_check_parameters()) + DC_engine::BEGIN_NONSTATIC,
                    std::begin(parameters_to_initiate)
                    ) &&
                std::equal(
                    std::begin(drive.to_check_parameters()) + DC_engine::BEGIN_NONSTATIC,
                    std::end(drive.to_check_parameters()),
                    std::begin(parameters_to_set)
                    )
                );
}

BOOST_AUTO_TEST_SUITE_END()
