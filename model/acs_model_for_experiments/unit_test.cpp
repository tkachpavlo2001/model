#define BOOST_TEST_MAIN

#define BOOST_TEST_DYN_LINK

#include<boost/test/unit_test.hpp>
#include"controlled_process.h"
#include<memory>
#include<algorithm>
#include<cmath>
#include<array>
#include<sstream>


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
                drive.to_check_parameters().size() == DC_engine::SIZE &&
                drive.to_check_parameters()[DC_engine::END_DC_ENGINE - 1] + 1 == DC_engine::SIZE &&
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

BOOST_AUTO_TEST_CASE(case_1_8_Verifying_to_set_element_parameters)
{
    DC_engine drive;

    double num = 0;
    std::vector<double> parameters_to_initiate = drive.to_check_parameters();
    std::vector<double> parameters_to_set = drive.to_check_parameters();

    // parameters_to_initiate filling
    for (auto i = std::begin(parameters_to_initiate); i < std::end(parameters_to_initiate); ++num, ++i)
        *i = 1 * num;
    // parameters_to_initiate setting
    drive.to_set_all_parameters(parameters_to_initiate);


    // parameters_to_set filling
    num = 0;
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1.01 * num;
    // parameters_to_set setting
    drive.to_set_element_parameters(parameters_to_set);

    parameters_to_set[DC_engine::TORQUE_OF_LOAD] =
            parameters_to_set[DC_engine::LOAD_K_0] +
            parameters_to_set[DC_engine::LOAD_K_1] * parameters_to_set[DC_engine::VELOCITY] +
            parameters_to_set[DC_engine::LOAD_K_2] * parameters_to_set[DC_engine::VELOCITY] * parameters_to_set[DC_engine::VELOCITY];
    parameters_to_set[DC_engine::MOMENT_OF_INERTIA] =
            parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] +
            parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_ENGINE];

    BOOST_REQUIRE(
                std::equal(
                    std::begin(drive.to_check_parameters()),
                    std::begin(drive.to_check_parameters()) + DC_engine::END_INTERFACE,
                    std::begin(parameters_to_initiate)
                    ) &&
                std::equal(
                    std::begin(drive.to_check_parameters()) + DC_engine::END_INTERFACE,
                    std::end(drive.to_check_parameters()),
                    std::begin(parameters_to_set) + DC_engine::END_INTERFACE
                    )
                );
}

BOOST_AUTO_TEST_CASE(case_1_9_Verifying_to_set_configurative_parameters)
{
    DC_engine drive;
    double num = 0;
    std::vector<double> parameters_to_set = std::vector<double>(DC_engine::SIZE, 0);
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1.1 * num;
    drive.to_set_configurative_parameters(parameters_to_set);
    BOOST_REQUIRE(
                std::equal(
                    drive.to_check_parameters().begin(),
                    drive.to_check_parameters().begin() + DC_engine::END_INTERFACE,
                    parameters_to_set.begin()
                    )
                );
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(DC_engine_calculations_testing)

BOOST_AUTO_TEST_CASE(case_2_1_Verifying_to_set_calculation_mode_and_to_check_calculation_mode)
{
    DC_engine drive;
    DC_engine::calculation_mode_states calculation_mode_enum = DC_engine::EULER;
    drive.to_set_calculation_mode(calculation_mode_enum);
    BOOST_REQUIRE_EQUAL(drive.to_check_calculation_mode(), calculation_mode_enum);
    calculation_mode_enum = DC_engine::RUNGE_KUTTA;
    drive.to_set_calculation_mode(calculation_mode_enum);
    BOOST_REQUIRE_EQUAL(drive.to_check_calculation_mode(), calculation_mode_enum);
}

BOOST_AUTO_TEST_CASE(case_2_2_calculations_for_the_final_value_statement_with_euler)
{
    DC_engine drive_with_euler;
    drive_with_euler.to_set_calculation_mode(DC_engine::EULER);
    // 4ITO100L1 engine and given parameters:
    double power_given = 1.5e3;
    double voltage_given = 220;
    double current_given = 19;
    double velocity_nominal_given_turnout_per_minute = 1500;
    //double velocity_max_given_turnout_per_minute = 4000;
    // calculated paremeters of the engine:
    double velocity_nominal_calculated_rad_per_second =
            velocity_nominal_given_turnout_per_minute *
            2 *
            M_PI /
            60;
    /*double velocity_max_calculated_rad_per_second =
            velocity_max_given_turnout_per_minute *
            2 *
            M_PI /
            60;*/
    double torque_nominal_calculated =
            power_given /
            velocity_nominal_calculated_rad_per_second;
    /*double torque_resistance_calculated =
            power_given /
            velocity_max_calculated_rad_per_second;*/
    double kf_calculated = torque_nominal_calculated / current_given;
    double electromotive_force = kf_calculated * velocity_nominal_calculated_rad_per_second;
    double electromotive_force_to_check = power_given / current_given;
    BOOST_REQUIRE_EQUAL(electromotive_force, electromotive_force_to_check);
    double resistance_calculated = ( voltage_given - electromotive_force ) / current_given;
    // suggested parameters of the engine
    double inductivity_suggested = 5e-4;   // 0.1 mH - 1 mH +-= 0.5 mH
    double inertia_suggested = 1e3;
    double inertia_load_suggested = 4e3;


    // chosen parameters for the engine modeling
    double voltage_chosen = voltage_given;
    double kf_chosen = kf_calculated;
    double resistance_chosen = resistance_calculated;
    double inertia_chosen = inertia_suggested * 10e-6;
    double inertia_load_chosen = inertia_load_suggested * 10e-7;
    double inductivity_chosen = inductivity_suggested;
    double kL_0 = torque_nominal_calculated * 0.9;
    double kL_1 = 0;
    // calculation settings
    double dt_chosen = 10e-5;
    double t_length = 8;

    std::array<double, DC_engine::SIZE> array_of_the_parameters_to_set = {0};
    array_of_the_parameters_to_set[DC_engine::DT] = dt_chosen;
    array_of_the_parameters_to_set[DC_engine::INPUT_SIGNAL] = voltage_chosen;
    array_of_the_parameters_to_set[DC_engine::KF] = kf_chosen;
    array_of_the_parameters_to_set[DC_engine::RESISTANCE] = resistance_chosen;
    array_of_the_parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_ENGINE] = inertia_chosen;
    array_of_the_parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = inertia_load_chosen;
    array_of_the_parameters_to_set[DC_engine::INDUCTIVITY] = inductivity_chosen;

    array_of_the_parameters_to_set[DC_engine::LOAD_K_0] = kL_0;
    array_of_the_parameters_to_set[DC_engine::LOAD_K_1] = kL_1;


    std::cout << "t\t\t\tTheta\t\t\tVelocity\t\t\tCurrent\t\t\tTorque\t\t\tLoad\t\t\tAcceleration\n";
    drive_with_euler.to_set_all_parameters(
                std::vector<double> (
                    std::begin(array_of_the_parameters_to_set),
                    std::end(array_of_the_parameters_to_set)
                    )
                );

    for (unsigned int t = 0; t <= static_cast<unsigned int>(t_length / dt_chosen); ++t)
    {
        drive_with_euler.to_calculate();
        if ( !( (t * 100) % static_cast<unsigned int>(t_length / dt_chosen) ) )
        {
            std::ostringstream out_string;
            std::vector<double> parameters_of_engine = drive_with_euler.to_check_parameters();
            out_string << static_cast<double>(t) * static_cast<double>(dt_chosen) << "\t\t\t"
                       << parameters_of_engine[DC_engine::THETA] << "\t\t\t"
                       << parameters_of_engine[DC_engine::OUTPUT_SIGNAL] << "\t\t\t"
                       << parameters_of_engine[DC_engine::CURRENT] << "\t\t\t"
                       << parameters_of_engine[DC_engine::CURRENT] * parameters_of_engine[DC_engine::KF] << "\t\t\t"
                       << parameters_of_engine[DC_engine::TORQUE_OF_LOAD] << "\t\t\t"
                       << parameters_of_engine[DC_engine::ACCELERATION] << "\t\t\t"
                       << std::endl;
            std::string string_to_out = out_string.str();
            for (auto i = std::begin(string_to_out); i < std::end(string_to_out); i++)
                if (*i == '.') *i = ',';
            std::cout << string_to_out << std::flush;
            string_to_out.clear();
        }
    }
    double velocity_calculated = voltage_chosen/kf_chosen - kL_0 * resistance_chosen / (kf_calculated * kf_calculated);
    std::cout << "Velocity calculated: " << velocity_calculated << std::endl;
    BOOST_CHECK_CLOSE_FRACTION(velocity_calculated, drive_with_euler.to_check_parameters()[DC_engine::VELOCITY], 1e-3);
}

BOOST_AUTO_TEST_CASE(case_2_2_calculations_for_the_final_value_statement_with_runge_kutta)
{
    DC_engine drive_with_runge_kutta;
    drive_with_runge_kutta.to_set_calculation_mode(DC_engine::RUNGE_KUTTA);
    // 4ITO100L1 engine and given parameters:
    double power_given = 1.5e3;
    double voltage_given = 220;
    double current_given = 19;
    double velocity_nominal_given_turnout_per_minute = 1500;
    //double velocity_max_given_turnout_per_minute = 4000;
    // calculated paremeters of the engine:
    double velocity_nominal_calculated_rad_per_second =
            velocity_nominal_given_turnout_per_minute *
            2 *
            M_PI /
            60;
    /*double velocity_max_calculated_rad_per_second =
            velocity_max_given_turnout_per_minute *
            2 *
            M_PI /
            60;*/
    double torque_nominal_calculated =
            power_given /
            velocity_nominal_calculated_rad_per_second;
    /*double torque_resistance_calculated =
            power_given /
            velocity_max_calculated_rad_per_second;*/
    double kf_calculated = torque_nominal_calculated / current_given;
    double electromotive_force = kf_calculated * velocity_nominal_calculated_rad_per_second;
    double electromotive_force_to_check = power_given / current_given;
    BOOST_REQUIRE_EQUAL(electromotive_force, electromotive_force_to_check);
    double resistance_calculated = ( voltage_given - electromotive_force ) / current_given;
    // suggested parameters of the engine
    double inductivity_suggested = 5e-4;   // 0.1 mH - 1 mH +-= 0.5 mH
    double inertia_suggested = 1e3;
    double inertia_load_suggested = 4e3;


    // chosen parameters for the engine modeling
    double voltage_chosen = voltage_given;
    double kf_chosen = kf_calculated;
    double resistance_chosen = resistance_calculated;
    double inertia_chosen = inertia_suggested * 10e-6;
    double inertia_load_chosen = inertia_load_suggested * 10e-7;
    double inductivity_chosen = inductivity_suggested;
    double kL_0 = torque_nominal_calculated * 0.9;
    double kL_1 = 0;
    // calculation settings
    double dt_chosen = 10e-5;
    double t_length = 8;

    std::array<double, DC_engine::SIZE> array_of_the_parameters_to_set = {0};
    array_of_the_parameters_to_set[DC_engine::DT] = dt_chosen;
    array_of_the_parameters_to_set[DC_engine::INPUT_SIGNAL] = voltage_chosen;
    array_of_the_parameters_to_set[DC_engine::KF] = kf_chosen;
    array_of_the_parameters_to_set[DC_engine::RESISTANCE] = resistance_chosen;
    array_of_the_parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_ENGINE] = inertia_chosen;
    array_of_the_parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = inertia_load_chosen;
    array_of_the_parameters_to_set[DC_engine::INDUCTIVITY] = inductivity_chosen;

    array_of_the_parameters_to_set[DC_engine::LOAD_K_0] = kL_0;
    array_of_the_parameters_to_set[DC_engine::LOAD_K_1] = kL_1;


    std::cout << "t\t\t\tTheta\t\t\tVelocity\t\t\tCurrent\t\t\tTorque\t\t\tLoad\t\t\tAcceleration\n";
    drive_with_runge_kutta.to_set_all_parameters(
                std::vector<double> (
                    std::begin(array_of_the_parameters_to_set),
                    std::end(array_of_the_parameters_to_set)
                    )
                );

    for (unsigned int t = 0; t <= static_cast<unsigned int>(t_length / dt_chosen); ++t)
    {
        drive_with_runge_kutta.to_calculate();
        if ( !( (t * 100) % static_cast<unsigned int>(t_length / dt_chosen) ) )
        {
            std::ostringstream out_string;
            std::vector<double> parameters_of_engine = drive_with_runge_kutta.to_check_parameters();
            out_string << static_cast<double>(t) * static_cast<double>(dt_chosen) << "\t\t\t"
                       << parameters_of_engine[DC_engine::THETA] << "\t\t\t"
                       << parameters_of_engine[DC_engine::OUTPUT_SIGNAL] << "\t\t\t"
                       << parameters_of_engine[DC_engine::CURRENT] << "\t\t\t"
                       << parameters_of_engine[DC_engine::CURRENT] * parameters_of_engine[DC_engine::KF] << "\t\t\t"
                       << parameters_of_engine[DC_engine::TORQUE_OF_LOAD] << "\t\t\t"
                       << parameters_of_engine[DC_engine::ACCELERATION] << "\t\t\t"
                       << std::endl;
            std::string string_to_out = out_string.str();
            for (auto i = std::begin(string_to_out); i < std::end(string_to_out); i++)
                if (*i == '.') *i = ',';
            std::cout << string_to_out << std::flush;
            string_to_out.clear();
        }
    }
    double velocity_calculated = voltage_chosen/kf_chosen - kL_0 * resistance_chosen / (kf_calculated * kf_calculated);
    std::cout << "Velocity calculated: " << velocity_calculated << std::endl;
    BOOST_CHECK_CLOSE_FRACTION(velocity_calculated, drive_with_runge_kutta.to_check_parameters()[DC_engine::VELOCITY], 1e-3);
}

BOOST_AUTO_TEST_SUITE_END()
