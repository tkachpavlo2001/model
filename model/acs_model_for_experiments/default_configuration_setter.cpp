#include "default_configuration_setter.h"
#include <cmath>
#include <iostream>
#include <array>

Default_configuration_setter::Default_configuration_setter()
{

}
void Default_configuration_setter::to_fill_process(std::shared_ptr<DC_engine> drive) const
{
    // https://servomotors.ru/documentation/electromotor/book1/book1p63.html
    // МП-12 2,5 220 1300 14,2 1,33 0,05
    //
    //
    //
    //
    drive->to_set_calculation_mode(DC_engine::RUNGE_KUTTA);
    // 4ITO100L1 engine and given parameters:
    double power_given = 2.5e3;
    double voltage_given = 220;
    double current_nominal_given = 14.2;
    double velocity_nominal_given_turnout_per_minute = 1300;
    double resistance_given = 1.33;
    double inertia_given = 0.05;
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
    double kf_calculated = torque_nominal_calculated / current_nominal_given;
    double electromotive_force = kf_calculated * velocity_nominal_calculated_rad_per_second;
    double electromotive_force_to_check = power_given / current_nominal_given;
    if (electromotive_force == electromotive_force_to_check) std::cout << "Drive is OK!\n";
    double resistance_calculated = ( voltage_given - electromotive_force ) / current_nominal_given;
    // suggested parameters of the engine
    double inductivity_suggested = 5e-4;   // 0.1 mH - 1 mH +-= 0.5 mH
    double inertia_load_suggested = inertia_given * 9;


    // chosen parameters for the engine modeling
    double voltage_chosen = voltage_given;
    double kf_chosen = kf_calculated;
    double resistance_chosen = resistance_calculated;
    double inertia_chosen = inertia_given;
    double inertia_load_chosen = inertia_load_suggested;
    double inductivity_chosen = inductivity_suggested;
    double kL_0 = torque_nominal_calculated * 0;
    double kL_1 = 0;
    double kL_exp_1 = 5;
    double kL_exp_2 = torque_nominal_calculated;
    // calculation settings
    double dt_chosen = 10e-5;
    double t_length = 8;

    std::array<double, DC_engine::SIZE> array_of_the_parameters_to_set = {0};
    array_of_the_parameters_to_set[DC_engine::DT] = dt_chosen;
    array_of_the_parameters_to_set[DC_engine::INPUT_SIGNAL] = 0;
    array_of_the_parameters_to_set[DC_engine::KF] = kf_chosen;
    array_of_the_parameters_to_set[DC_engine::RESISTANCE] = resistance_chosen;
    array_of_the_parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_ENGINE] = inertia_chosen;
    array_of_the_parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = inertia_load_chosen;
    array_of_the_parameters_to_set[DC_engine::INDUCTIVITY] = inductivity_chosen;

    array_of_the_parameters_to_set[DC_engine::LOAD_K_EXP_CURVATURE] = kL_exp_1;
    array_of_the_parameters_to_set[DC_engine::LOAD_K_EXP_LIMIT] = kL_exp_2;

    drive->to_set_all_parameters({array_of_the_parameters_to_set.begin(), array_of_the_parameters_to_set.end()});

    drive->to_receive_input_signal(220);

    //process_test_infinitive_run(drive,velocity_nominal_calculated_rad_per_second,torque_nominal_calculated); // do not enabale! only to debug
}


void Default_configuration_setter::to_set_elements_parameters(
        std::shared_ptr<Reference_signal_definder_static> _definder,
        std::shared_ptr<PID_regulator> _regulator,
        std::shared_ptr<DC_source> _source,
        std::shared_ptr<DC_engine> _process
        ) const
{
    _definder->to_set_signal(100); //definder->to_set_signal(100);
    _regulator->to_set_koefficients(5, 2e-5, 4);
    _source->to_set_max_voltage(220);
    _source->to_set_min_voltage(-220);
    this->to_fill_process(_process);
}

void Default_configuration_setter::to_set_experiment_parameters(std::shared_ptr<Experiment_executor> _experiment) const
{
    _experiment->to_set_dt(1e-5);
    _experiment->to_set_t_begin(0);
    _experiment->to_set_t_length(10); //experiment->to_set_t_length(10);
    _experiment->to_set_time_to_registrate(1e-1); //experiment->to_set_time_to_registrate(1e-1);
}
