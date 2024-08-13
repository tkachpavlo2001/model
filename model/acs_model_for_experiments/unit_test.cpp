#define BOOST_TEST_MAIN

#define BOOST_TEST_DYN_LINK

#include<boost/test/unit_test.hpp>

#include"controlled_process.h"
#include"regulator.h"
#include"reference_signal_definder_static.h"
#include"dc_source.h"
#include"automated_control_system.h"
#include"registrator.h"
#include"experiment_executor.h"
#include"default_configuration_setter.h"

#include<memory>
#include<algorithm>
#include<cmath>
#include<array>
#include<sstream>
#include<fstream>
#include<cctype>



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

std::vector<double> calculation_regulator_check(PID_regulator & _controller, const std::vector<double> & _signals)
{
    std::vector<double> u_of_t;
    for (auto selected_signal : _signals)
    {
        _controller.to_receive_input_signal(selected_signal);
        _controller.to_calculate();
        u_of_t.push_back(_controller.to_check_parameters()[PID_regulator::OUTPUT_SIGNAL]);
    }
    return u_of_t;
}


BOOST_AUTO_TEST_SUITE(DC_engine_interface_testing_and_abstract_class_part_testing)

BOOST_AUTO_TEST_CASE(case_1_1_DC_engine_object_creating)
{
    std::shared_ptr<DC_engine> p_drive = nullptr;
    p_drive = std::make_shared<DC_engine>();
    BOOST_REQUIRE(p_drive != nullptr);
}

BOOST_AUTO_TEST_CASE(case_1_2_Verifying_to_set_dt_inherited)
{
    DC_engine drive;
    drive.to_set_dt(2.2);
    BOOST_REQUIRE(drive.to_check_parameters()[DC_engine::DT] == 2.2);
}

BOOST_AUTO_TEST_CASE(case_1_3_Verifying_to_receive_input_signal_inherited)
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

BOOST_AUTO_TEST_CASE(case_1_5_Verifying_to_check_parameters_and_to_get_parameters_inherited)
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

BOOST_AUTO_TEST_CASE(case_1_6_Verifying_amount_of_parameters_methods_inherited)
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

BOOST_AUTO_TEST_CASE(case_1_7_Verifying_to_get_output_signal_inherited)
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

BOOST_AUTO_TEST_CASE(case_1_10_Verifying_to_check_the_type)
{
    std::shared_ptr<DC_engine> p_drive = nullptr;
    p_drive = std::make_shared<DC_engine>();
    BOOST_REQUIRE(p_drive->to_check_the_type() == DC_engine::PROCESS);
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


BOOST_AUTO_TEST_SUITE(PID_regulator_interface_testing)

BOOST_AUTO_TEST_CASE(case_3_1_PID_regulator_object_creating)
{
    std::shared_ptr<PID_regulator> controller = nullptr;
    controller = std::make_shared<PID_regulator>();
    BOOST_CHECK(controller != nullptr);
}

BOOST_AUTO_TEST_CASE(case_3_2_Verifying_to_set_koefficients)
{
    PID_regulator controller;
    controller.to_set_koefficients(1.1, 2.2, 3.3);
    BOOST_CHECK(controller.to_check_parameters()[PID_regulator::K_P] == 1.1);
    BOOST_CHECK(controller.to_check_parameters()[PID_regulator::K_I] == 2.2);
    BOOST_CHECK(controller.to_check_parameters()[PID_regulator::K_D] == 3.3);
}

BOOST_AUTO_TEST_CASE(case_3_2_Verifying_to_receive_reference_signal)
{
    PID_regulator controller;
    controller.to_receive_reference_signal(1.11);
    BOOST_CHECK(controller.to_check_parameters()[PID_regulator::REFERENCE_SIGNAL] == 1.11);
}

BOOST_AUTO_TEST_CASE(case_3_3_Verifying_to_verify_amount_of_parameters)
{
    PID_regulator controller;
    double num = 0;
    std::vector<double> parameters_to_set = controller.to_check_parameters();
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1 * num;
    controller.to_set_all_parameters(parameters_to_set);
    BOOST_WARN_MESSAGE(controller.to_check_amount_of_parameters() == PID_regulator::SIZE, "to_check_amount_of_parameters() -- problem");
    BOOST_WARN_MESSAGE(controller.to_verify_amount_of_parameters(), "to_verify_amount_of_parameters() -- problem");
    BOOST_REQUIRE(
                controller.to_check_amount_of_parameters() == PID_regulator::SIZE &&
                controller.to_check_parameters().size() == PID_regulator::SIZE &&
                controller.to_check_parameters()[PID_regulator::END_REGULATOR - 1] + 1 == PID_regulator::SIZE &&
                controller.to_verify_amount_of_parameters()
                );
}

BOOST_AUTO_TEST_CASE(case_3_4_Verifying_to_set_all_parameters)
{
    PID_regulator controller;

    double num = 0;
    std::vector<double> parameters_to_initiate = controller.to_check_parameters();

    // parameters_to_initiate filling
    for (auto i = std::begin(parameters_to_initiate); i < std::end(parameters_to_initiate); ++num, ++i)
        *i = 1 * num;
    // parameters_to_initiate setting
    controller.to_set_all_parameters(parameters_to_initiate);

    BOOST_REQUIRE(
                std::equal(
                    std::begin(controller.to_check_parameters()),
                    std::end(controller.to_check_parameters()),
                    std::begin(parameters_to_initiate)
                    )
                );
}

BOOST_AUTO_TEST_CASE(case_3_5_Verifying_to_set_element_parameters)
{
    PID_regulator controller;

    double num = 0;
    std::vector<double> parameters_to_initiate = controller.to_check_parameters();
    std::vector<double> parameters_to_set = controller.to_check_parameters();

    // parameters_to_initiate filling
    for (auto i = std::begin(parameters_to_initiate); i < std::end(parameters_to_initiate); ++num, ++i)
        *i = 1 * num;
    // parameters_to_initiate setting
    controller.to_set_all_parameters(parameters_to_initiate);


    // parameters_to_set filling
    num = 0;
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1.01 * num;
    // parameters_to_set setting
    controller.to_set_element_parameters(parameters_to_set);

    BOOST_REQUIRE(
                std::equal(
                    std::begin(controller.to_check_parameters()),
                    std::begin(controller.to_check_parameters()) + PID_regulator::END_INTERFACE,
                    std::begin(parameters_to_initiate)
                    ) &&
                std::equal(
                    std::begin(controller.to_check_parameters()) + PID_regulator::END_INTERFACE,
                    std::end(controller.to_check_parameters()),
                    std::begin(parameters_to_set) + PID_regulator::END_INTERFACE
                    )
                );
}

BOOST_AUTO_TEST_CASE(case_3_6_Verifying_to_check_the_type)
{
    std::shared_ptr<PID_regulator> controller = nullptr;
    controller = std::make_shared<PID_regulator>();
    BOOST_REQUIRE(controller->to_check_the_type() == PID_regulator::REGULATOR);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(PID_regulator_calculation_testing)

BOOST_AUTO_TEST_CASE(case_4_1_verifying_of_the_P_regulation)
{
    PID_regulator controller;
    double reference_signal = 10;
    controller.to_set_dt(2);
    controller.to_receive_reference_signal(reference_signal);
    controller.to_receive_input_signal(0);
    controller.to_set_koefficients(2);
    std::vector<double> y_of_t {0, 1, 2, 3, 5, 7, 9, 11, 13, 17};
    std::vector<double> u_of_t_to_check = calculation_regulator_check(controller, y_of_t);
    if (verbose_mode_of_calculations) for (auto i : u_of_t_to_check)
        std::cout << i << std::endl;
    if (verbose_mode_of_calculations) std::cout << std::endl << std::endl;
    std::vector<double> u_of_t_reference;
    for (auto selected_signal : y_of_t)
    {
        u_of_t_reference.push_back(
                        (
                            reference_signal -
                            selected_signal
                        ) * controller.to_check_parameters()[PID_regulator::K_P]
                    );
    }
    for(
        int i = 0;
        std::begin(u_of_t_reference) + i < std::end(u_of_t_reference);
        ++i
        )
        BOOST_CHECK_EQUAL( *(std::begin(u_of_t_reference)+i), *(std::begin(u_of_t_to_check)+i) );
}

BOOST_AUTO_TEST_CASE(case_4_2_verifying_of_the_I_regulation)
{
    PID_regulator controller;
    double reference_signal = 10;
    controller.to_set_dt(2);
    controller.to_receive_reference_signal(reference_signal);
    controller.to_receive_input_signal(0);
    controller.to_set_koefficients(0, 1.5);
    std::vector<double> y_of_t {0, 1, 2, 3, 5, 7, 9, 11, 13, 17};
    std::vector<double> u_of_t_to_check = calculation_regulator_check(controller, y_of_t);
    if (verbose_mode_of_calculations) for (auto i : u_of_t_to_check)
        std::cout << i << std::endl;
    if (verbose_mode_of_calculations) std::cout << std::endl << std::endl;
    std::vector<double> u_of_t_reference;
    double sum_of_errors;
    for (auto selected_signal : y_of_t)
    {
        sum_of_errors += reference_signal - selected_signal;
        u_of_t_reference.push_back( sum_of_errors * controller.to_check_parameters()[PID_regulator::K_I] );
    }
    for(
        int i = 0;
        std::begin(u_of_t_reference) + i < std::end(u_of_t_reference);
        ++i
        )
        BOOST_CHECK_EQUAL( *(std::begin(u_of_t_reference)+i), *(std::begin(u_of_t_to_check)+i) );
}

BOOST_AUTO_TEST_CASE(case_4_3_verifying_of_the_the_whole_PID_regulator_calculation_D_regulations_in_particular)
{
    PID_regulator controller;
    double reference_signal = 10;
    controller.to_set_dt(2);
    controller.to_receive_reference_signal(reference_signal);
    controller.to_receive_input_signal(0);
    controller.to_set_koefficients(2, 1, 0.5);
    std::vector<double> y_of_t {0, 1, 2, 3, 5, 7, 9, 11, 13, 17};
    std::vector<double> u_of_t_to_check = calculation_regulator_check(controller, y_of_t);
    if (verbose_mode_of_calculations) for (auto i : u_of_t_to_check)
        std::cout << i << std::endl;
    if (verbose_mode_of_calculations) std::cout << std::endl << std::endl;
    std::vector<double> u_of_t_reference;
    double error;
    double sum_of_errors;
    for (auto selected_signal : y_of_t)
    {
        static double previous_error = 0;
        double derror_dt;
        error = reference_signal - selected_signal;
        sum_of_errors += error;
        derror_dt = (error - previous_error) / controller.to_check_parameters()[PID_regulator::DT];

        double p_of_t = controller.to_check_parameters()[PID_regulator::K_P] * error;
        double i_of_t = controller.to_check_parameters()[PID_regulator::K_I] * sum_of_errors;
        double d_of_t = controller.to_check_parameters()[PID_regulator::K_D] * derror_dt;

        u_of_t_reference.push_back( p_of_t + i_of_t + d_of_t );

        previous_error = error;
    }
    for(
        int i = 0;
        std::begin(u_of_t_reference) + i < std::end(u_of_t_reference);
        ++i
        )
        BOOST_CHECK_EQUAL( *(std::begin(u_of_t_reference)+i), *(std::begin(u_of_t_to_check)+i) );
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(reference_signal_definder_static_calculation_testing)

BOOST_AUTO_TEST_CASE(case_5_1_reference_signal_definder_static_object_creating)
{
    std::shared_ptr<Reference_signal_definder_static> definder = nullptr;
    definder = std::make_shared<Reference_signal_definder_static>();
    BOOST_CHECK(definder != nullptr);
}

BOOST_AUTO_TEST_CASE(case_5_2_Verifying_to_set_koefficients)
{
    Reference_signal_definder_static definder;
    definder.to_set_signal(1.1);
    BOOST_CHECK(definder.to_check_parameters()[Reference_signal_definder_static::OUTPUT_SIGNAL] == 1.1);
}

BOOST_AUTO_TEST_CASE(case_5_3_Verifying_to_verify_amount_of_parameters)
{
    Reference_signal_definder_static definder;
    double num = 0;
    std::vector<double> parameters_to_set = definder.to_check_parameters();
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1 * num;
    definder.to_set_all_parameters(parameters_to_set);
    BOOST_REQUIRE(definder.to_check_amount_of_parameters() == Reference_signal_definder_static::SIZE);
    BOOST_REQUIRE(definder.to_check_parameters().size() == Reference_signal_definder_static::SIZE);
    BOOST_REQUIRE(definder.to_check_parameters()[Reference_signal_definder_static::END_INTERFACE - 1] + 1 == Reference_signal_definder_static::SIZE);
    BOOST_REQUIRE(definder.to_verify_amount_of_parameters());
}

BOOST_AUTO_TEST_CASE(case_5_4_Verifying_to_set_all_parameters)
{
    Reference_signal_definder_static definder;

    double num = 0;
    std::vector<double> parameters_to_initiate = definder.to_check_parameters();

    // parameters_to_initiate filling
    for (auto i = std::begin(parameters_to_initiate); i < std::end(parameters_to_initiate); ++num, ++i)
        *i = 1 * num;
    // parameters_to_initiate setting
    definder.to_set_all_parameters(parameters_to_initiate);

    BOOST_REQUIRE(
                std::equal(
                    std::begin(definder.to_check_parameters()),
                    std::end(definder.to_check_parameters()),
                    std::begin(parameters_to_initiate)
                    )
                );
}

BOOST_AUTO_TEST_CASE(case_5_5_Verifying_to_set_element_parameters)
{
    Reference_signal_definder_static definder;

    double num = 0;
    std::vector<double> parameters_to_initiate = definder.to_check_parameters();
    std::vector<double> parameters_to_set = definder.to_check_parameters();

    // parameters_to_initiate filling
    for (auto i = std::begin(parameters_to_initiate); i < std::end(parameters_to_initiate); ++num, ++i)
        *i = 1 * num;
    // parameters_to_initiate setting
    definder.to_set_all_parameters(parameters_to_initiate);


    // parameters_to_set filling
    num = 0;
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1.01 * num;
    // parameters_to_set setting
    BOOST_REQUIRE(!definder.to_set_element_parameters(parameters_to_set));

    BOOST_REQUIRE(
                std::equal(
                    std::begin(definder.to_check_parameters()),
                    std::begin(definder.to_check_parameters()) + Reference_signal_definder_static::END_INTERFACE,
                    std::begin(parameters_to_initiate)
                    ) &&
                std::equal(
                    std::begin(definder.to_check_parameters()) + Reference_signal_definder_static::END_INTERFACE,
                    std::end(definder.to_check_parameters()),
                    std::begin(parameters_to_set) + Reference_signal_definder_static::END_INTERFACE
                    )
                );
}

BOOST_AUTO_TEST_CASE(case_5_6_Verifying_to_check_the_type)
{
    std::shared_ptr<Reference_signal_definder_static> definder = nullptr;
    definder = std::make_shared<Reference_signal_definder_static>();
    BOOST_REQUIRE(definder->to_check_the_type() == Reference_signal_definder_static::REFERENCE_SIGNAL_DEFINDER);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(DC_source_testing)

BOOST_AUTO_TEST_CASE(case_6_1_reference_DC_source_object_creating)
{
    std::shared_ptr<DC_source> source = nullptr;
    source = std::make_shared<DC_source>();
    BOOST_CHECK(source != nullptr);
}

BOOST_AUTO_TEST_CASE(case_6_2_Verifying_to_set_limit_voltage)
{
    DC_source source;
    source.to_set_max_voltage(1.1);
    source.to_set_min_voltage(-2.2);
    BOOST_CHECK(source.to_check_parameters()[DC_source::MAX_VOLTAGE] == 1.1);
    BOOST_CHECK(source.to_check_parameters()[DC_source::MIN_VOLTAGE] == -2.2);
}

BOOST_AUTO_TEST_CASE(case_6_3_Verifying_to_verify_amount_of_parameters)
{
    DC_source source;
    double num = 0;
    std::vector<double> parameters_to_set = source.to_check_parameters();
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1 * num;
    source.to_set_all_parameters(parameters_to_set);
    BOOST_REQUIRE(source.to_check_amount_of_parameters() == DC_source::SIZE);
    BOOST_REQUIRE(source.to_check_parameters().size() == DC_source::SIZE);
    BOOST_REQUIRE(source.to_check_parameters()[DC_source::END_SOURCE - 1] + 1 == DC_source::SIZE);
    BOOST_REQUIRE(source.to_verify_amount_of_parameters());
}

BOOST_AUTO_TEST_CASE(case_6_4_Verifying_to_set_all_parameters)
{
    DC_source source;

    double num = 0;
    std::vector<double> parameters_to_initiate = source.to_check_parameters();

    // parameters_to_initiate filling
    for (auto i = std::begin(parameters_to_initiate); i < std::end(parameters_to_initiate); ++num, ++i)
        *i = 1 * num;
    // parameters_to_initiate setting
    source.to_set_all_parameters(parameters_to_initiate);

    BOOST_REQUIRE(
                std::equal(
                    std::begin(source.to_check_parameters()),
                    std::end(source.to_check_parameters()),
                    std::begin(parameters_to_initiate)
                    )
                );
}

BOOST_AUTO_TEST_CASE(case_6_5_Verifying_to_set_element_parameters)
{
    DC_source source;

    double num = 0;
    std::vector<double> parameters_to_initiate = source.to_check_parameters();
    std::vector<double> parameters_to_set = source.to_check_parameters();

    // parameters_to_initiate filling
    for (auto i = std::begin(parameters_to_initiate); i < std::end(parameters_to_initiate); ++num, ++i)
        *i = 1 * num;
    // parameters_to_initiate setting
    source.to_set_all_parameters(parameters_to_initiate);


    // parameters_to_set filling
    num = 0;
    for (auto i = std::begin(parameters_to_set); i < std::end(parameters_to_set); ++num, ++i)
        *i = 1.01 * num;
    // parameters_to_set setting
    BOOST_REQUIRE(!source.to_set_element_parameters(parameters_to_set));

    BOOST_REQUIRE(
                std::equal(
                    std::begin(source.to_check_parameters()),
                    std::begin(source.to_check_parameters()) + DC_source::END_INTERFACE,
                    std::begin(parameters_to_initiate)
                    ) &&
                std::equal(
                    std::begin(source.to_check_parameters()) + DC_source::END_INTERFACE,
                    std::end(source.to_check_parameters()),
                    std::begin(parameters_to_set) + DC_source::END_INTERFACE
                    )
                );
}

BOOST_AUTO_TEST_CASE(case_6_6_Verifying_to_check_the_type)
{
    std::shared_ptr<DC_source> source = nullptr;
    source = std::make_shared<DC_source>();
    BOOST_REQUIRE(source->to_check_the_type() == DC_source::ENERGY_SOURCE);
}

BOOST_AUTO_TEST_CASE(case_6_7_Verifying_to_calculate)
{
    DC_source source;
    source.to_set_max_voltage(11.1);
    source.to_set_min_voltage(-5.5);
    source.to_receive_input_signal(10);
    source.to_calculate();
    BOOST_REQUIRE_EQUAL(source.to_get_output_signal(), 10);

    source.to_receive_input_signal(20);
    source.to_calculate();
    BOOST_REQUIRE_EQUAL(source.to_get_output_signal(), 11.1);

    source.to_receive_input_signal(-10);
    source.to_calculate();
    BOOST_REQUIRE_EQUAL(source.to_get_output_signal(), -5.5);
}

BOOST_AUTO_TEST_CASE(case_5_7_Verifying_to_set_limit_voltage_incapsulation)
{
    DC_source source;
    source.to_set_max_voltage(1);
    source.to_set_min_voltage(2);
    BOOST_REQUIRE_EQUAL(source.to_check_parameters()[DC_source::MIN_VOLTAGE], 1);
    source.to_set_max_voltage(-1);
    BOOST_REQUIRE_EQUAL(source.to_check_parameters()[DC_source::MIN_VOLTAGE], 1);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Automated_control_system_testing)

BOOST_AUTO_TEST_CASE(case_7_1_Automated_control_system_creating)
{
    std::shared_ptr<Automated_control_system> acs_model = nullptr;
    acs_model = std::make_shared<Automated_control_system>();
    BOOST_CHECK(acs_model != nullptr);
}

BOOST_AUTO_TEST_CASE(case_7_2_Verifying_to_check_elements_and_its_attributes_initialising)
{
    Automated_control_system acs_model;
    BOOST_REQUIRE_EQUAL(acs_model.to_check_elements().size(), 4);
    for (auto & i : acs_model.to_check_elements())
        BOOST_REQUIRE_EQUAL(i, nullptr);
    DC_source source;
    acs_model.to_mount_the_element(source); // HERE
    for (auto & i : acs_model.to_check_elements())
        if(i != nullptr)
            BOOST_REQUIRE(i->to_check_the_type() == DC_source::ENERGY_SOURCE);
}

BOOST_AUTO_TEST_CASE(case_7_3_Verifying_to_mount_the_element_first_part)
{
    Automated_control_system acs_model;
    std::vector<Automated_control_system_element_interface *> elements_line(4);
    elements_line[0] = new DC_source;
    elements_line[1] = new DC_engine;
    elements_line[2] = new Reference_signal_definder_static;
    elements_line[3] = new PID_regulator;
    for (auto & i : elements_line)
        acs_model.to_mount_the_element(i);
    elements_line[1]->to_get_parameters()[Automated_control_system_element_interface::DT] = (0+1) * 1.1;
    elements_line[3]->to_get_parameters()[Automated_control_system_element_interface::DT] = (1+1) * 1.1;
    elements_line[2]->to_get_parameters()[Automated_control_system_element_interface::DT] = (2+1) * 1.1;
    elements_line[0]->to_get_parameters()[Automated_control_system_element_interface::DT] = (3+1) * 1.1;
    for (auto & i : acs_model.to_check_elements())
    {
        if(i->to_check_the_type() == Automated_control_system_element_interface::PROCESS)
            BOOST_REQUIRE_CLOSE_FRACTION(i->to_check_parameters()[Automated_control_system_element_interface::DT], (0+1) * 1.1, 0.001);
        if(i->to_check_the_type() == Automated_control_system_element_interface::REGULATOR)
            BOOST_REQUIRE_CLOSE_FRACTION(i->to_check_parameters()[Automated_control_system_element_interface::DT], (1+1) * 1.1, 0.001);
        if(i->to_check_the_type() == Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER)
            BOOST_REQUIRE_CLOSE_FRACTION(i->to_check_parameters()[Automated_control_system_element_interface::DT], (2+1) * 1.1, 0.001);
        if(i->to_check_the_type() == Automated_control_system_element_interface::ENERGY_SOURCE)
            BOOST_REQUIRE_CLOSE_FRACTION(i->to_check_parameters()[Automated_control_system_element_interface::DT], (3+1) * 1.1, 0.001);
    }


    for (auto & i : elements_line)
        delete i;
}

BOOST_AUTO_TEST_CASE(case_7_4_Verifying_to_mount_the_element_second_part)
{
    Automated_control_system acs_model;
    std::vector<Automated_control_system_element_interface *> elements_line(4);
    elements_line[0] = new DC_source;
    elements_line[1] = new DC_engine;
    elements_line[2] = new Reference_signal_definder_static;
    elements_line[3] = new PID_regulator;
    for (auto & i : elements_line)
        acs_model.to_mount_the_element(i);
    elements_line[1]->to_get_parameters()[Automated_control_system_element_interface::DT] = (0+1) * 1.1;
    elements_line[3]->to_get_parameters()[Automated_control_system_element_interface::DT] = (1+1) * 1.1;
    elements_line[2]->to_get_parameters()[Automated_control_system_element_interface::DT] = (2+1) * 1.1;
    elements_line[0]->to_get_parameters()[Automated_control_system_element_interface::DT] = (3+1) * 1.1;


    DC_source new_source;
    DC_engine new_drive;
    new_source.to_set_dt(5.5);
    new_drive.to_set_dt(6.6);
    acs_model.to_mount_the_element(new_source);
    acs_model.to_mount_the_element(new_drive);
    BOOST_REQUIRE_EQUAL(acs_model.to_check_elements().size(), 4);
    for (auto & i : acs_model.to_check_elements())
    {
        if(i->to_check_the_type() == Automated_control_system_element_interface::PROCESS)
            BOOST_REQUIRE_CLOSE_FRACTION(i->to_check_parameters()[Automated_control_system_element_interface::DT], 6.6, 0.001);
        if(i->to_check_the_type() == Automated_control_system_element_interface::REGULATOR)
            BOOST_REQUIRE_CLOSE_FRACTION(i->to_check_parameters()[Automated_control_system_element_interface::DT], 2.2, 0.001);
        if(i->to_check_the_type() == Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER)
            BOOST_REQUIRE_CLOSE_FRACTION(i->to_check_parameters()[Automated_control_system_element_interface::DT], 3.3, 0.001);
        if(i->to_check_the_type() == Automated_control_system_element_interface::ENERGY_SOURCE)
            BOOST_REQUIRE_CLOSE_FRACTION(i->to_check_parameters()[Automated_control_system_element_interface::DT], 5.5, 0.001);
    }



    for (auto & i : elements_line)
        delete i;
}

BOOST_AUTO_TEST_CASE(case_7_5_Verifying_to_set_dt)
{
    Automated_control_system acs_model;
    std::vector<Automated_control_system_element_interface *> elements_line(4);
    elements_line[0] = new DC_source;
    elements_line[1] = new DC_engine;
    elements_line[2] = new Reference_signal_definder_static;
    elements_line[3] = new PID_regulator;
    for(auto & i : elements_line)
        acs_model.to_mount_the_element(i);
    acs_model.to_set_dt(0.01);
    for (auto & i : elements_line)
        BOOST_REQUIRE(i->to_check_parameters()[Automated_control_system_element_interface::DT] == 0.01);
    acs_model.to_set_dt(2);
    for (auto & i : elements_line)
        BOOST_REQUIRE(i->to_check_parameters()[Automated_control_system_element_interface::DT] == 2);
}

BOOST_AUTO_TEST_CASE(case_7_6_Verifying_to_check_certain_element_ALL_methods_of_this_kind)
{
    Automated_control_system acs_model;
    std::vector<Automated_control_system_element_interface *> elements_line(4);
    elements_line[0] = new DC_source;
    elements_line[1] = new DC_engine;
    elements_line[2] = new Reference_signal_definder_static;
    elements_line[3] = new PID_regulator;
    for (auto & i : elements_line)
        acs_model.to_mount_the_element(i);
    elements_line[1]->to_get_parameters()[Automated_control_system_element_interface::DT] = (0+1) * 1.1;
    elements_line[3]->to_get_parameters()[Automated_control_system_element_interface::DT] = (1+1) * 1.1;
    elements_line[2]->to_get_parameters()[Automated_control_system_element_interface::DT] = (2+1) * 1.1;
    elements_line[0]->to_get_parameters()[Automated_control_system_element_interface::DT] = (3+1) * 1.1;


    DC_source new_source;
    DC_engine new_drive;
    new_source.to_set_dt(5.5);
    new_drive.to_set_dt(6.6);
    acs_model.to_mount_the_element(new_source);
    acs_model.to_mount_the_element(new_drive);

    BOOST_REQUIRE_CLOSE_FRACTION(acs_model.to_check_process()->to_check_parameters()[Automated_control_system_element_interface::DT], 6.6, 0.001);
    BOOST_REQUIRE_CLOSE_FRACTION(acs_model.to_check_regulator()->to_check_parameters()[Automated_control_system_element_interface::DT], 2.2, 0.001);
    BOOST_REQUIRE_CLOSE_FRACTION(acs_model.to_check_definder()->to_check_parameters()[Automated_control_system_element_interface::DT], 3.3, 0.001);
    BOOST_REQUIRE_CLOSE_FRACTION(acs_model.to_check_source()->to_check_parameters()[Automated_control_system_element_interface::DT], 5.5, 0.001);

    BOOST_REQUIRE_CLOSE_FRACTION(acs_model.to_check_certain_element(Automated_control_system_element_interface::PROCESS)->to_check_parameters()[Automated_control_system_element_interface::DT], 6.6, 0.001);
    BOOST_REQUIRE_CLOSE_FRACTION(acs_model.to_check_certain_element(Automated_control_system_element_interface::REGULATOR)->to_check_parameters()[Automated_control_system_element_interface::DT], 2.2, 0.001);
    BOOST_REQUIRE_CLOSE_FRACTION(acs_model.to_check_certain_element(Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER)->to_check_parameters()[Automated_control_system_element_interface::DT], 3.3, 0.001);
    BOOST_REQUIRE_CLOSE_FRACTION(acs_model.to_check_certain_element(Automated_control_system_element_interface::ENERGY_SOURCE)->to_check_parameters()[Automated_control_system_element_interface::DT], 5.5, 0.001);

    Automated_control_system acs_model_2;
    std::vector<const Automated_control_system_element_interface *> p_elements = acs_model_2.to_check_elements();

    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_process(), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_regulator(), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_definder(), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_source(), nullptr);

    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_certain_element(Automated_control_system_element_interface::PROCESS), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_certain_element(Automated_control_system_element_interface::REGULATOR), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_certain_element(Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_certain_element(Automated_control_system_element_interface::ENERGY_SOURCE), nullptr);


    acs_model_2.to_mount_the_element(elements_line[1]);

    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_regulator(), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_definder(), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_source(), nullptr);

    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_certain_element(Automated_control_system_element_interface::REGULATOR), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_certain_element(Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_certain_element(Automated_control_system_element_interface::ENERGY_SOURCE), nullptr);


    acs_model_2.to_mount_the_element(elements_line[3]);

    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_definder(), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_source(), nullptr);

    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_certain_element(Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER), nullptr);
    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_certain_element(Automated_control_system_element_interface::ENERGY_SOURCE), nullptr);


    acs_model_2.to_mount_the_element(elements_line[2]);

    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_source(), nullptr);

    BOOST_REQUIRE_EQUAL(acs_model_2.to_check_certain_element(Automated_control_system_element_interface::ENERGY_SOURCE), nullptr);




    for (auto & i : elements_line)
        delete i;
}

BOOST_AUTO_TEST_CASE(case_7_7_Verifying_to_check_ordered_elements)
{
    Automated_control_system acs_model;
    std::vector<Automated_control_system_element_interface *> elements_line(4);
    elements_line[0] = new DC_source;
    elements_line[1] = new DC_engine;
    elements_line[2] = new Reference_signal_definder_static;
    elements_line[3] = new PID_regulator;
    for (auto & i : elements_line)
        acs_model.to_mount_the_element(i);
    auto out_of_method = acs_model.to_check_ordered_elements();
    BOOST_REQUIRE_EQUAL(out_of_method[0], elements_line[2]);
    BOOST_REQUIRE_EQUAL(out_of_method[1], elements_line[3]);
    BOOST_REQUIRE_EQUAL(out_of_method[2], elements_line[0]);
    BOOST_REQUIRE_EQUAL(out_of_method[3], elements_line[1]);

    for ( auto & i : elements_line)
        delete i;
}

BOOST_AUTO_TEST_CASE(case_7_8_Verifying_t_functions)
{
    Automated_control_system acs_model;
    acs_model.to_set_t(2.2);
    BOOST_REQUIRE_EQUAL(acs_model.to_check_t(), 2.2);
    acs_model.to_set_dt(3.3);
    BOOST_REQUIRE_EQUAL(acs_model.to_check_dt(), 3.3);
    acs_model.to_calculate();
    BOOST_REQUIRE_EQUAL(acs_model.to_check_t(), 5.5);
}

BOOST_AUTO_TEST_CASE(case_7_8_Verifying_nullptr_run)
{
    Automated_control_system acs_model;
    BOOST_REQUIRE_EQUAL(acs_model.to_check_dt(), 0);
    BOOST_REQUIRE_EQUAL(acs_model.to_check_t(), 0);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Registrator_testing)

BOOST_AUTO_TEST_CASE(case_8_1_Registrator_creating_and_the_file_opening)
{
    const char * file_name_of_the_case = "unit_test_run_case_8_1";

    {
        Registrator_to_txt_file * p_fout = nullptr;
        p_fout = new Registrator_to_txt_file;
        BOOST_REQUIRE(p_fout != nullptr);
        delete p_fout;
    }

    {
        Registrator_to_txt_file fout;
        fout.to_set_name_of_file(file_name_of_the_case);
        BOOST_REQUIRE(!fout.is_open());
        Automated_control_system acs_model;
        DC_engine * p_drive = new DC_engine;
        acs_model.to_mount_the_element(p_drive);
        fout << acs_model;
        BOOST_REQUIRE(fout.is_open());
        delete p_drive;
    }
}

BOOST_AUTO_TEST_CASE(case_8_2_Its_function_verifying)
{
    const char * file_name_of_the_case = "unit_test_run_case_8_2";
    Registrator_to_txt_file fout;
    fout.to_set_name_of_file(file_name_of_the_case);

    Automated_control_system acs_model;

    std::vector<Automated_control_system_element_interface *> elements_line(4);
    elements_line[0] = new Reference_signal_definder_static;
    elements_line[1] = new PID_regulator;
    elements_line[2] = new DC_source;
    elements_line[3] = new DC_engine;

    unsigned int k = 0;
    double num_to_multiply = 1.01;
    double num_to_add = 100.001;
    double t = 1.1;
    double dt = 2.2;

    acs_model.to_set_t(t);
    acs_model.to_set_dt(dt);

    for(auto & i : elements_line)
        for(auto & j : i->to_get_parameters())
        {
            j = num_to_multiply * k;
            ++k;
        }

    for(auto & i : elements_line)
        acs_model.to_mount_the_element(i);

    fout << acs_model;

    for(auto & i : elements_line)
        for(auto & j : i->to_get_parameters())
        {
            j += num_to_add;
        }

    fout << acs_model;

    char temp_char = '\0';
    double temp_num = 0;
    bool first_time = true;
    k = 0;
    std::fstream fin(file_name_of_the_case + std::string(".txt"));
    while( temp_char != '\n')
    {
        temp_char = fin.peek();
        if(temp_char == '\n')
            continue;
        if(isdigit(temp_char))
        {
            fin >> temp_num;
            if(first_time) { BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t, 0.001); first_time = false; }
            else
            {
                BOOST_REQUIRE_CLOSE_FRACTION(temp_num, k * num_to_multiply, 0.001);
                ++k;
            }
        }
        fin.get();
    }
    fin.get();
    first_time = true;
    while( temp_char != '\n')
    {
        temp_char = fin.peek();
        if(temp_char == '\n')
            continue;
        if(isdigit(temp_char))
        {
            fin >> temp_num;
            if(first_time) { BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t, 0.001); first_time = false; }
            else
            {
                BOOST_REQUIRE_CLOSE_FRACTION(temp_num, k * num_to_multiply, 0.001);
                ++k;
            }
        }
        fin.get();
    }
    fin.get();

}

BOOST_AUTO_TEST_CASE(case_8_3_Registrator_to_std_vector_creating)
{
    Registrator_to_std_vector * recorder = nullptr;
    recorder = new Registrator_to_std_vector;
    BOOST_REQUIRE(recorder != nullptr);
    Automated_control_system acs_model;
    Registrator_to_std_vector & r = *recorder;
    r << acs_model;

    delete recorder;
}

BOOST_AUTO_TEST_CASE(case_8_4_Registrator_to_std_vector_function_verifying)
{
    Default_configuration_setter setter;
    std::shared_ptr<Automated_control_system_paralleled> p_acs_model = std::make_shared<Automated_control_system_paralleled>();
    Automated_control_system & r_acs_model = *p_acs_model.get();
    Registrator_to_std_vector recorder;

    Reference_signal_definder_static * definder = new Reference_signal_definder_static;
    PID_regulator * regulator = new PID_regulator;
    DC_source * source = new DC_source;
    DC_engine * process = new DC_engine;

    setter.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    p_acs_model->to_mount_the_element(definder);
    p_acs_model->to_mount_the_element(regulator);
    p_acs_model->to_mount_the_element(source);
    p_acs_model->to_mount_the_element(process);

    std::vector<double> records;

    recorder.to_set_vector(records);

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(p_acs_model.get());
    experiment->to_set_dt(1e-4);

    definder->to_set_signal(100);
    recorder << r_acs_model;


    for (int i = 0; i < 1000; ++i) r_acs_model.to_calculate();
    recorder << r_acs_model;
    for (int i = 0; i < 1000; ++i) r_acs_model.to_calculate();
    recorder << r_acs_model;

    BOOST_REQUIRE_EQUAL(records.at(0), 0);
    if (verbose_mode_of_calculations) for (auto i : records) std::cout << i << std::endl;

}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Experiment_executor_testing)

BOOST_AUTO_TEST_CASE(case_9_1_Experiment_executor_creating)
{
    Experiment_executor * experiment = nullptr;
    experiment = new Experiment_executor;
    BOOST_REQUIRE(experiment != nullptr);
    delete experiment;
}

BOOST_AUTO_TEST_CASE(case_9_2_Verifying_nullptr_run)
{
    Experiment_executor experiment;
    experiment.to_set_result_title("case_9_2_Verifying_nullptr_run");
    experiment.to_run();
}

BOOST_AUTO_TEST_CASE(case_9_3_Verifying_interval_initialized)
{
    const char * title = "case_9_{3,4,5,6}_Verifying_run";
    Automated_control_system acs_model;
    double t_init = 1;
    acs_model.to_set_t(t_init);
    acs_model.to_set_dt(0.01);
    Experiment_executor experiment(&acs_model);
    Experiment_executor experiment_2;
    experiment.to_get_model_to_run(&acs_model);

    double length = 2;
    double interval = 0.5;
    double fraction = 0.01;

    experiment.to_set_t_length(length);
    experiment.to_set_time_to_registrate(interval);
    experiment.to_set_result_title(title);
    experiment.to_run();

    double temp_num;
    char temp_char = '\0';
    std::ifstream fin(std::string(title) + std::string(".txt"));
    BOOST_REQUIRE(fin.is_open());
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init, 0.0001);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init + interval, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init + interval * 2, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin.close();
}

BOOST_AUTO_TEST_CASE(case_9_4_Verifying_interval_set)
{
    const char * title = "case_9_{3,4,5,6}_Verifying_run";
    Automated_control_system acs_model;
    double t_init = 1;
    acs_model.to_set_t(t_init);
    acs_model.to_set_dt(0.01);
    Experiment_executor experiment(&acs_model);
    Experiment_executor experiment_2;
    experiment_2.to_get_model_to_run(&acs_model);

    double length = 2;
    double interval = 0.5;
    double fraction = 0.01;

    experiment.to_set_t_length(length);
    experiment.to_set_time_to_registrate(interval);
    experiment.to_set_result_title(title);
    experiment.to_run();

    double temp_num;
    char temp_char = '\0';
    std::ifstream fin(std::string(title) + std::string(".txt"));

    experiment_2.to_set_t_length(length);
    experiment_2.to_set_time_to_registrate(interval);
    experiment_2.to_run();

    temp_char = '\0';
    BOOST_REQUIRE(fin.is_open());
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init + interval, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init + interval * 2, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin.close();
}

BOOST_AUTO_TEST_CASE(case_9_5_Verifying_amount_initialized)
{
    const char * title = "case_9_{3,4,5,6}_Verifying_run";
    Automated_control_system acs_model;
    double t_init = 1;
    acs_model.to_set_t(t_init);
    acs_model.to_set_dt(0.01);
    Experiment_executor experiment(&acs_model);
    experiment.to_get_model_to_run(&acs_model);

    double length = 2;
    double interval = 0.5;
    double fraction = 0.01;

    experiment.to_set_t_length(length);
    experiment.to_set_time_to_registrate(interval);
    experiment.to_set_result_title(title);
    experiment.to_run();

    double temp_num;
    char temp_char = '\0';

    std::ifstream fin(std::string(title) + std::string(".txt"));
    temp_char = '\0';
    BOOST_REQUIRE(fin.is_open());
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init + interval, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init + interval * 2, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin.close();
}

BOOST_AUTO_TEST_CASE(case_9_6_Verifying_amount_set)
{
    const char * title = "case_9_{3,4,5,6}_Verifying_run";
    Automated_control_system acs_model;
    double t_init = 1;
    acs_model.to_set_t(t_init);
    acs_model.to_set_dt(0.01);
    Experiment_executor experiment(&acs_model);
    Experiment_executor experiment_2;
    experiment_2.to_set_result_title(title);
    experiment_2.to_get_model_to_run(&acs_model);

    double length = 2;
    double interval = 0.5;
    double fraction = 0.01;


    double temp_num;
    char temp_char = '\0';

    experiment_2.to_set_t_length(length);
    experiment_2.to_set_amount_of_registrations(length / interval);
    experiment_2.to_set_result_title(title);
    experiment_2.to_run();

    std::ifstream fin(std::string(title) + std::string(".txt"));
    temp_char = '\0';
    BOOST_REQUIRE(fin.is_open());
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init + interval, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin >> temp_num;
    BOOST_REQUIRE_CLOSE_FRACTION(temp_num, t_init + interval * 2, fraction);
    while (temp_char != '\n')
        temp_char = fin.get();
    fin.close();
}

BOOST_AUTO_TEST_CASE(case_9_7_Verifying_Experiment_executor_for_fitness_function_with_varied_reference_signal_creating)
{
    Experiment_executor_for_fitness_function_with_varied_reference_signal * p_executor = nullptr;
    p_executor = new Experiment_executor_for_fitness_function_with_varied_reference_signal;
    BOOST_REQUIRE(p_executor != 0);
    delete p_executor;
}

BOOST_AUTO_TEST_CASE(case_9_8_Verifying_Experiment_executor_for_fitness_function_with_varied_reference_signal_functional)
{
    std::vector<double> results_0;
    std::vector<double> results_1;
    Experiment_executor_for_fitness_function experiment_0;
    Experiment_executor_for_fitness_function_with_varied_reference_signal experiment_1;

    std::shared_ptr<Reference_signal_definder_static> definder_0 = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<Reference_signal_definder_static> definder_1 = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator_0 = std::make_shared<PID_regulator>();
    std::shared_ptr<PID_regulator> regulator_1 = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source_0 = std::make_shared<DC_source>();
    std::shared_ptr<DC_source> source_1 = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process_0 = std::make_shared<DC_engine>();
    std::shared_ptr<DC_engine> process_1 = std::make_shared<DC_engine>();

    Default_configuration_setter default_configuration_setter_obj;
    default_configuration_setter_obj.to_set_elements_parameters(
                definder_0,
                regulator_0,
                source_0,
                process_0
                );
    default_configuration_setter_obj.to_set_elements_parameters(
                definder_1,
                regulator_1,
                source_1,
                process_1
                );

    std::shared_ptr<Automated_control_system_paralleled> acs_model_0 = std::make_shared<Automated_control_system_paralleled>();
    acs_model_0->to_mount_the_element(definder_0.get());
    acs_model_0->to_mount_the_element(regulator_0.get());
    acs_model_0->to_mount_the_element(source_0.get());
    acs_model_0->to_mount_the_element(process_0.get());
    std::shared_ptr<Automated_control_system_paralleled> acs_model_1 = std::make_shared<Automated_control_system_paralleled>();
    acs_model_1->to_mount_the_element(definder_1.get());
    acs_model_1->to_mount_the_element(regulator_1.get());
    acs_model_1->to_mount_the_element(source_1.get());
    acs_model_1->to_mount_the_element(process_1.get());

    experiment_0.to_set_vector(results_0);
    experiment_0.to_get_model_to_run(acs_model_0.get());
    experiment_1.to_set_vector(results_1);
    experiment_1.to_get_model_to_run(acs_model_1.get());
    default_configuration_setter_obj.to_set_experiment_parameters(&experiment_0);
    default_configuration_setter_obj.to_set_experiment_parameters(&experiment_1);

    experiment_0.to_set_t_length(10);
    experiment_1.to_set_t_length(20);

    definder_0->to_set_signal(100);
    experiment_0.to_run();
    definder_0->to_set_signal(50);
    experiment_0.to_run();

    experiment_1.to_set_varied_diapasone_min_max(50, 100);
    experiment_1.to_run();

    if (verbose_mode_of_calculations)for(unsigned int i = 0; i < results_0.size(); ++i) std::cout << results_0.at(i) << '\t' << results_1.at(i) << std::endl;

    BOOST_REQUIRE_EQUAL(results_0.size(), results_1.size());

    if (verbose_mode_of_calculations)for(unsigned int i = 0; i < results_0.size(); ++i) BOOST_REQUIRE_EQUAL(results_0[i], results_1[i]);
}

BOOST_AUTO_TEST_SUITE_END()
