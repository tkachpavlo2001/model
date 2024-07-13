#define BOOST_TEST_MAIN

#define BOOST_TEST_DYN_LINK

#include<boost/test/unit_test.hpp>
#include"controlled_process.h"
#include<memory>
#include<algorithm>
#include<cmath>
#include<array>
#include<sstream>

bool verbose_mode_of_calculations = false;

class ask_tester
{
public:
    ask_tester(bool & _mode_parameter)
    {
        char answer = '\0';
        std::cout << "Do you want the calculations to be verbose? Y/N: " << std::flush;
        bool to_backspace = false;
        while (std::tolower(answer) != 'y' && std::tolower(answer) != 'n')
        {
            if (to_backspace)
                std::cout << "Y/N: ";
            std::cin.get(answer);
            while ( std::cin.get() != '\n' )
                continue;
            to_backspace = true;
        }
        if (std::tolower(answer) == 'y') _mode_parameter = true;
    }
};

ask_tester question(verbose_mode_of_calculations);

double calculation_loop(DC_engine & _drive, double _t_length, double _dt_chosen, bool _show_mode = verbose_mode_of_calculations)
{
    if (_show_mode) std::cout << "\n\nt\t\t\tTheta\t\t\tVelocity\t\t\tCurrent\t\t\tTorque\t\t\tLoad\n";

    for (unsigned int t = 0; t <= static_cast<unsigned int>(_t_length / _dt_chosen); ++t)
    {
        _drive.to_calculate();
        if (_show_mode && !( (t * 100) % static_cast<unsigned int>(_t_length / _dt_chosen) ) )
        {
            std::ostringstream out_string;
            std::vector<double> parameters_of_engine = _drive.to_check_parameters();
            out_string << static_cast<double>(t) * static_cast<double>(_dt_chosen) << "\t\t\t"
                       << parameters_of_engine[DC_engine::THETA] << "\t\t\t"
                       << parameters_of_engine[DC_engine::OUTPUT_SIGNAL] << "\t\t\t"
                       << parameters_of_engine[DC_engine::CURRENT] << "\t\t\t"
                       << parameters_of_engine[DC_engine::CURRENT] * parameters_of_engine[DC_engine::KF] << "\t\t\t"
                       << parameters_of_engine[DC_engine::TORQUE_OF_LOAD] << "\t\t\t"
                       << std::endl;
            std::string string_to_out = out_string.str();
            for (auto i = std::begin(string_to_out); i < std::end(string_to_out); i++)
                if (*i == '.') *i = ',';
            std::cout << string_to_out << std::flush;
            string_to_out.clear();
        }
    }
    if (_show_mode) std::cout << "\n\n";
    std::vector<double> parameters_of_engine = _drive.to_check_parameters();
    double velocity_calculated =
            parameters_of_engine[DC_engine::VOLTAGE] /
            parameters_of_engine[DC_engine::KF] -
            parameters_of_engine[DC_engine::LOAD_K_0] *
            parameters_of_engine[DC_engine::RESISTANCE] / (
                parameters_of_engine[DC_engine::KF] *
                parameters_of_engine[DC_engine::KF]
                );
    if (_show_mode)
    {
        std::cout << "Velocity calculated: " << velocity_calculated << std::endl;
    }
    return velocity_calculated;
}

std::vector<std::pair<double, double>> calculation_loop_both_methods(DC_engine & _drive_1, DC_engine & _drive_2, double _t_length, double _dt_chosen, const std::vector<double> & momets_of_measurments, bool _show_mode = verbose_mode_of_calculations)
{
    std::vector<std::pair<double, double>> records;
    if (_show_mode) std::cout << "\n\nt\t\t\tTheta\t\t\tVelocity\t\t\tCurrent\t\t\tTorque\t\t\tLoad\n";
    for (unsigned int t = 0; t <= static_cast<unsigned int>(_t_length / _dt_chosen); ++t)
    {
        _drive_1.to_calculate();
        _drive_2.to_calculate();
        for (auto i : momets_of_measurments)
            if ( t == static_cast<unsigned int>(i / _dt_chosen) )
            {
                //std::cout << "THERE:\n";
                records.push_back(std::pair<double, double>(_drive_1.to_check_parameters()[DC_engine::VELOCITY], _drive_2.to_check_parameters()[DC_engine::VELOCITY]));
            }

        //if ( _show_mode && t <= static_cast<unsigned int>(0.3/**momets_of_measurments.rbegin()*/ / _dt_chosen) )
        if (_show_mode && !( (t * 100) % static_cast<unsigned int>(_t_length / _dt_chosen) ) )
        {
            std::ostringstream out_string;
            std::vector<double> parameters_of_engine_1 = _drive_1.to_check_parameters();
            std::vector<double> parameters_of_engine_2 = _drive_2.to_check_parameters();
            out_string << static_cast<double>(t) * static_cast<double>(_dt_chosen) << "\t\t\t"
                       << parameters_of_engine_1[DC_engine::THETA] << '|' << parameters_of_engine_1[DC_engine::THETA] << "\t\t\t"
                       << parameters_of_engine_1[DC_engine::OUTPUT_SIGNAL] << '|' << parameters_of_engine_1[DC_engine::OUTPUT_SIGNAL] <<  "\t\t\t"
                       << parameters_of_engine_1[DC_engine::CURRENT] << '|' << parameters_of_engine_1[DC_engine::CURRENT] <<  "\t\t\t"
                       << parameters_of_engine_1[DC_engine::CURRENT] * parameters_of_engine_2[DC_engine::KF] << '|'
                            << parameters_of_engine_1[DC_engine::CURRENT] * parameters_of_engine_2[DC_engine::KF] << "\t\t\t"
                       << parameters_of_engine_1[DC_engine::TORQUE_OF_LOAD] << '|' << parameters_of_engine_2[DC_engine::TORQUE_OF_LOAD] <<  "\t\t\t"
                       << std::endl;
            std::string string_to_out = out_string.str();
            for (auto i = std::begin(string_to_out); i < std::end(string_to_out); i++)
                if (*i == '.') *i = ',';
            std::cout << string_to_out << std::flush;
            string_to_out.clear();
        }
    }
    return records;
}

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
            (
                (parameters_to_set[DC_engine::VELOCITY] > 0) ?
                parameters_to_set[DC_engine::LOAD_K_EXP_LIMIT] *
                (1 - std::exp( - parameters_to_set[DC_engine::VELOCITY] * parameters_to_set[DC_engine::LOAD_K_EXP_CURVATURE]))
                :
                parameters_to_set[DC_engine::LOAD_K_EXP_LIMIT] *
                (- 1 + std::exp(parameters_to_set[DC_engine::VELOCITY] * parameters_to_set[DC_engine::LOAD_K_EXP_CURVATURE]))
            );
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
            (
                (parameters_to_set[DC_engine::VELOCITY] > 0) ?
                parameters_to_set[DC_engine::LOAD_K_EXP_LIMIT] *
                (1 - std::exp( - parameters_to_set[DC_engine::VELOCITY] * parameters_to_set[DC_engine::LOAD_K_EXP_CURVATURE]))
                :
                parameters_to_set[DC_engine::LOAD_K_EXP_LIMIT] *
                (- 1 + std::exp(parameters_to_set[DC_engine::VELOCITY] * parameters_to_set[DC_engine::LOAD_K_EXP_CURVATURE]))
            );
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


    drive_with_euler.to_set_all_parameters(
                std::vector<double> (
                    std::begin(array_of_the_parameters_to_set),
                    std::end(array_of_the_parameters_to_set)
                    )
                );

    double velocity_calculated_with_static_method = calculation_loop(drive_with_euler, t_length, dt_chosen);

    BOOST_CHECK_CLOSE_FRACTION(velocity_calculated_with_static_method, drive_with_euler.to_check_parameters()[DC_engine::VELOCITY], 1e-3);
}

BOOST_AUTO_TEST_CASE(case_2_3_calculations_for_the_final_value_statement_with_runge_kutta)
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


    drive_with_runge_kutta.to_set_all_parameters(
                std::vector<double> (
                    std::begin(array_of_the_parameters_to_set),
                    std::end(array_of_the_parameters_to_set)
                    )
                );


    calculation_loop(drive_with_runge_kutta, t_length, dt_chosen);

    double velocity_calculated_with_static_method = calculation_loop(drive_with_runge_kutta, t_length, dt_chosen);


    BOOST_CHECK_CLOSE_FRACTION(velocity_calculated_with_static_method, drive_with_runge_kutta.to_check_parameters()[DC_engine::VELOCITY], 1e-3);
}

BOOST_AUTO_TEST_CASE(case_2_4_comparation_of_the_both_methods_at_static_load)
{
    DC_engine drive_with_euler;
    DC_engine drive_with_runge_kutta;
    drive_with_euler.to_set_calculation_mode(DC_engine::EULER);
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

    double torque_nominal_calculated =
            power_given /
            velocity_nominal_calculated_rad_per_second;
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


    drive_with_euler.to_set_all_parameters(
                std::vector<double> (
                    std::begin(array_of_the_parameters_to_set),
                    std::end(array_of_the_parameters_to_set)
                    )
                );
    drive_with_runge_kutta.to_set_all_parameters(
                std::vector<double> (
                    std::begin(array_of_the_parameters_to_set),
                    std::end(array_of_the_parameters_to_set)
                    )
                );

    std::vector<double> moment_of_measurments;
    for(int i = 1 ; i < 10; ++i)
        moment_of_measurments.push_back(i / 10.0);

    std::vector<std::pair<double, double>> records = calculation_loop_both_methods(
                drive_with_euler,
                drive_with_runge_kutta,
                t_length,
                dt_chosen,
                moment_of_measurments
                );
    BOOST_REQUIRE( records.size() );
    int j = 0;
    for ( auto i : records )
    {
        if (verbose_mode_of_calculations) std::cout << '#' << j << std::endl;
        ++j;
        BOOST_REQUIRE_CLOSE_FRACTION( (records.at(0).first) , (records.at(0).second) , std::fabs(20) / 2 );
    }

    drive_with_euler.to_get_parameters()[DC_engine::INPUT_SIGNAL] = 100;
    drive_with_runge_kutta.to_get_parameters()[DC_engine::INPUT_SIGNAL] = 100;
    records = calculation_loop_both_methods(
                drive_with_euler,
                drive_with_runge_kutta,
                t_length,
                dt_chosen,
                moment_of_measurments
                );
    BOOST_REQUIRE( records.size() );
    j = 0;
    for ( auto i : records )
    {
        if (verbose_mode_of_calculations) std::cout << '#' << j << std::endl;
        ++j;
        BOOST_REQUIRE_CLOSE_FRACTION( (records.at(0).first) , (records.at(0).second) , std::fabs(20) / 2 );
    }
}

BOOST_AUTO_TEST_CASE(case_2_5_comparation_of_the_both_methods_at_nonstatic_load)
{
    DC_engine drive_with_euler;
    DC_engine drive_with_runge_kutta;
    drive_with_euler.to_set_calculation_mode(DC_engine::EULER);
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

    double torque_nominal_calculated =
            power_given /
            velocity_nominal_calculated_rad_per_second;
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
    double kL_1 = kL_0 * 0.1;
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


    drive_with_euler.to_set_all_parameters(
                std::vector<double> (
                    std::begin(array_of_the_parameters_to_set),
                    std::end(array_of_the_parameters_to_set)
                    )
                );
    drive_with_runge_kutta.to_set_all_parameters(
                std::vector<double> (
                    std::begin(array_of_the_parameters_to_set),
                    std::end(array_of_the_parameters_to_set)
                    )
                );

    std::vector<double> moment_of_measurments;
    for(int i = 1 ; i < 10; ++i)
        moment_of_measurments.push_back(i / 10.0);

    std::vector<std::pair<double, double>> records = calculation_loop_both_methods(
                drive_with_euler,
                drive_with_runge_kutta,
                t_length,
                dt_chosen,
                moment_of_measurments
                );
    BOOST_REQUIRE( records.size() );
    int j = 0;
    for ( auto i : records )
    {
        if (verbose_mode_of_calculations) std::cout << '#' << j << std::endl;
        ++j;
        BOOST_REQUIRE_CLOSE_FRACTION( (records.at(0).first) , (records.at(0).second) , std::fabs(20) / 2 );
    }

    drive_with_euler.to_get_parameters()[DC_engine::INPUT_SIGNAL] = 100;
    drive_with_runge_kutta.to_get_parameters()[DC_engine::INPUT_SIGNAL] = 100;
    records = calculation_loop_both_methods(
                drive_with_euler,
                drive_with_runge_kutta,
                t_length,
                dt_chosen,
                moment_of_measurments
                );
    BOOST_REQUIRE( records.size() );
    j = 0;
    for ( auto i : records )
    {
        if (verbose_mode_of_calculations) std::cout << '#' << j << std::endl;
        ++j;
        BOOST_REQUIRE_CLOSE_FRACTION( (records.at(0).first) , (records.at(0).second) , std::fabs(20) / 2 );
    }
}

BOOST_AUTO_TEST_CASE(case_2_6_comparation_of_the_both_methods_at_exponantal_load)
{
    DC_engine drive_with_euler;
    DC_engine drive_with_runge_kutta;
    drive_with_euler.to_set_calculation_mode(DC_engine::EULER);
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

    double torque_nominal_calculated =
            power_given /
            velocity_nominal_calculated_rad_per_second;
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
    double kL_0 = 0;
    double kL_1 = 0;
    double kL_exp_lim = torque_nominal_calculated * 0.9;
    double kL_exp_curv = 0.5 / velocity_nominal_calculated_rad_per_second ;
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

    array_of_the_parameters_to_set[DC_engine::LOAD_K_EXP_LIMIT] = kL_exp_lim;
    array_of_the_parameters_to_set[DC_engine::LOAD_K_EXP_CURVATURE] = kL_exp_curv;


    drive_with_euler.to_set_all_parameters(
                std::vector<double> (
                    std::begin(array_of_the_parameters_to_set),
                    std::end(array_of_the_parameters_to_set)
                    )
                );
    drive_with_runge_kutta.to_set_all_parameters(
                std::vector<double> (
                    std::begin(array_of_the_parameters_to_set),
                    std::end(array_of_the_parameters_to_set)
                    )
                );

    std::vector<double> moment_of_measurments;
    for(int i = 1 ; i < 10; ++i)
        moment_of_measurments.push_back(i / 10.0);

    std::vector<std::pair<double, double>> records = calculation_loop_both_methods(
                drive_with_euler,
                drive_with_runge_kutta,
                t_length,
                dt_chosen,
                moment_of_measurments
                );
    BOOST_REQUIRE( records.size() );
    int j = 0;
    for ( auto i : records )
    {
        if (verbose_mode_of_calculations) std::cout << '#' << j << std::endl;
        ++j;
        BOOST_REQUIRE_CLOSE_FRACTION( (records.at(0).first) , (records.at(0).second) , std::fabs(20) / 2 );
    }

    drive_with_euler.to_get_parameters()[DC_engine::INPUT_SIGNAL] = -220;
    drive_with_runge_kutta.to_get_parameters()[DC_engine::INPUT_SIGNAL] = -220;
    records = calculation_loop_both_methods(
                drive_with_euler,
                drive_with_runge_kutta,
                t_length,
                dt_chosen,
                moment_of_measurments
                );
    BOOST_REQUIRE( records.size() );
    j = 0;
    for ( auto i : records )
    {
        if (verbose_mode_of_calculations) std::cout << '#' << j << std::endl;
        ++j;
        BOOST_REQUIRE_CLOSE_FRACTION( (records.at(0).first) , (records.at(0).second) , std::fabs(20) / 2 );
    }
}


BOOST_AUTO_TEST_SUITE_END()


