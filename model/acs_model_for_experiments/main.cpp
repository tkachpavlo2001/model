#include <iostream>
#include <memory>
#include "automated_control_system.h"
#include "experiment_executor.h"

#include <cmath>

using namespace std;

void first_try();
void reference_atempt();
void third_try();

int main()
{
    cout << "Hello World!" << endl;
    third_try();
    return 0;
}


void to_fill_process(std::shared_ptr<DC_engine> drive);
void to_p_regulate_1();
void to_pi_regulate_1();
void to_pid_regulate_1();
void first_try()
{
    //to_p_regulate_1();
    std::cout << std::endl;
    //to_pi_regulate_1();
    std::cout << std::endl;
    to_pid_regulate_1();
    std::cout << std::endl;
}
void to_p_regulate_reference_atempt();
void to_pi_regulate_reference_atempt();
void to_pid_regulate_reference_atempt();
void reference_atempt()
{
    to_p_regulate_reference_atempt();
    std::cout << std::endl;
    to_pi_regulate_reference_atempt();
    std::cout << std::endl;
    to_pid_regulate_reference_atempt();
    std::cout << std::endl;
}
void to_p_regulate_3();
void to_pi_regulate_3();
void to_pid_regulate_3();
void third_try()
{
    to_p_regulate_3();
    std::cout << std::endl;
    to_pi_regulate_3();
    std::cout << std::endl;
    to_pid_regulate_3();
    std::cout << std::endl;
}


void to_p_regulate_1()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    to_fill_process(process);
    definder->to_set_signal(100);
    regulator->to_set_koefficients(1, 0, 0);
    source->to_set_max_voltage(220);
    source->to_set_min_voltage(-220);

    std::shared_ptr<Automated_control_system> acs_model = std::make_shared<Automated_control_system>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_dt(1e-5);
    experiment->to_set_t_begin(0);
    experiment->to_set_t_length(10);
    experiment->to_set_time_to_registrate(1e-1);
    experiment->to_set_result_title("The moment of true_P_regulation");
    experiment->to_run();
}
void to_pi_regulate_1()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    to_fill_process(process);
    definder->to_set_signal(100);
    regulator->to_set_koefficients(1, 1e-3, 0);
    source->to_set_max_voltage(220);
    source->to_set_min_voltage(-220);

    std::shared_ptr<Automated_control_system> acs_model = std::make_shared<Automated_control_system>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_dt(1e-5);
    experiment->to_set_t_begin(0);
    experiment->to_set_t_length(10);
    experiment->to_set_time_to_registrate(1e-1);
    experiment->to_set_result_title("The moment of true_PI_regulation");
    experiment->to_run();
}
void to_pid_regulate_1()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    to_fill_process(process);
    definder->to_set_signal(100); //definder->to_set_signal(100);
    regulator->to_set_koefficients(1, 1e-3, 1e-2); //regulator->to_set_koefficients(1, 1e-3, 0);
    source->to_set_max_voltage(220);
    source->to_set_min_voltage(-220);

    std::shared_ptr<Automated_control_system> acs_model = std::make_shared<Automated_control_system>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_dt(1e-5);
    experiment->to_set_t_begin(0);
    experiment->to_set_t_length(10); //experiment->to_set_t_length(10);
    experiment->to_set_time_to_registrate(1e-1); //experiment->to_set_time_to_registrate(1e-1);
    experiment->to_set_result_title("The moment of true_PID_regulation");
    experiment->to_run();
}

void to_p_regulate_reference_atempt()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    to_fill_process(process);
    definder->to_set_signal(100);
    regulator->to_set_koefficients(5, 0, 0);
    source->to_set_max_voltage(220);
    source->to_set_min_voltage(-220);

    std::shared_ptr<Automated_control_system> acs_model = std::make_shared<Automated_control_system>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_dt(1e-5);
    experiment->to_set_t_begin(0);
    experiment->to_set_t_length(10);
    experiment->to_set_time_to_registrate(1e-1);
    experiment->to_set_result_title("The moment of true_P_regulation");
    experiment->to_run();
}
void to_pi_regulate_reference_atempt()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    to_fill_process(process);
    definder->to_set_signal(100);
    regulator->to_set_koefficients(5, 5e-5, 0);
    source->to_set_max_voltage(220);
    source->to_set_min_voltage(-220);

    std::shared_ptr<Automated_control_system> acs_model = std::make_shared<Automated_control_system>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_dt(1e-5);
    experiment->to_set_t_begin(0);
    experiment->to_set_t_length(10);
    experiment->to_set_time_to_registrate(1e-1);
    experiment->to_set_result_title("The moment of true_PI_regulation");
    experiment->to_run();
}
void to_pid_regulate_reference_atempt()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    to_fill_process(process);
    definder->to_set_signal(100); //definder->to_set_signal(100);
    regulator->to_set_koefficients(5, 2e-5, 4);
    source->to_set_max_voltage(220);
    source->to_set_min_voltage(-220);

    std::shared_ptr<Automated_control_system> acs_model = std::make_shared<Automated_control_system>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_dt(1e-5);
    experiment->to_set_t_begin(0);
    experiment->to_set_t_length(10); //experiment->to_set_t_length(10);
    experiment->to_set_time_to_registrate(1e-1); //experiment->to_set_time_to_registrate(1e-1);
    experiment->to_set_result_title("The moment of true_PID_regulation");
    experiment->to_run();
}

void to_p_regulate_3()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    to_fill_process(process);
    definder->to_set_signal(100);
    regulator->to_set_koefficients(5, 0, 0);
    source->to_set_max_voltage(220);
    source->to_set_min_voltage(-220);

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_dt(1e-5);
    experiment->to_set_t_begin(0);
    experiment->to_set_t_length(10);
    experiment->to_set_time_to_registrate(1e-1);
    experiment->to_set_result_title("The moment of true_P_regulation");
    experiment->to_run();
}
void to_pi_regulate_3()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    to_fill_process(process);
    definder->to_set_signal(100);
    regulator->to_set_koefficients(5, 5e-5, 0);
    source->to_set_max_voltage(220);
    source->to_set_min_voltage(-220);

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_dt(1e-5);
    experiment->to_set_t_begin(0);
    experiment->to_set_t_length(10);
    experiment->to_set_time_to_registrate(1e-1);
    experiment->to_set_result_title("The moment of true_PI_regulation");
    experiment->to_run();
}
void to_pid_regulate_3()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    to_fill_process(process);
    definder->to_set_signal(100); //definder->to_set_signal(100);
    regulator->to_set_koefficients(5, 2e-5, 4);
    source->to_set_max_voltage(220);
    source->to_set_min_voltage(-220);

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_dt(1e-5);
    experiment->to_set_t_begin(0);
    experiment->to_set_t_length(10); //experiment->to_set_t_length(10);
    experiment->to_set_time_to_registrate(1e-1); //experiment->to_set_time_to_registrate(1e-1);
    experiment->to_set_result_title("The moment of true_PID_regulation");
    experiment->to_run();
}


void process_test_infinitive_run(std::shared_ptr<DC_engine> drive, double, double);
void to_fill_process(std::shared_ptr<DC_engine> drive)
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
void process_test_infinitive_run(std::shared_ptr<DC_engine> drive, double velocity, double torque)
{
    std::cout << velocity << '\t' << torque << std::endl;
    double time = 0;
    while(1)
    {
        std::cout << time << '\t';
        for (auto & i : drive->to_check_parameters())
            if (
                    &i == &(drive->to_check_parameters()[DC_engine::VELOCITY])
                    ||
                    &i == &(drive->to_check_parameters()[DC_engine::VOLTAGE])
                    ||
                    &i == &(drive->to_check_parameters()[DC_engine::TORQUE])
                    ||
                    &i == &(drive->to_check_parameters()[DC_engine::TORQUE_OF_LOAD])
                )
                std::cout << i << '\t';
        for (auto & i : drive->to_check_parameters())
            if (
                    &i == &(drive->to_check_parameters()[DC_engine::TORQUE])
                )
                std::cout << i << '\t';
        std::cout << std::endl << std::endl;
        drive->to_calculate();
        time += drive->to_check_parameters()[DC_engine::DT];
    }
}
