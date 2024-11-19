#include <iostream>
#include <memory>
#include "automated_control_system.h"
#include "experiment_executor.h"
#include "default_configuration_setter.h"
#include "regulator_tuner.h"

#include <cmath>

using namespace std;

const Default_configuration_setter default_configuration_setter_obj;

void to_pid_regulate_3();
void to_pid_regulate_4();
void to_check_registrator_to_std_vector();
void to_check_fitness_f();
void to_check_regulator_tunner();
void to_check_my_regulator_tunner();
void to_check_my_regulator_tunner_in_new_shell();
void to_check_my_regulator_tunner_automated_manual();
void to_run_reference_instance();

int main()
{
    cout << "Hello World!" << endl;
    //to_pid_regulate_3();
    //to_pid_regulate_4();
    //to_check_fitness_f();
    //to_check_registrator_to_std_vector();
    //to_check_regulator_tunner();
    //to_check_my_regulator_tunner();
    //to_check_my_regulator_tunner_in_new_shell();
    to_check_my_regulator_tunner_automated_manual();
    //to_run_reference_instance();
    return 0;
}

void to_pid_regulate_3()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_result_title("The moment of true_PID_regulation");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment);

    experiment->to_run();
}

void to_pid_regulate_4()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::shared_ptr<Experiment_executor_for_fitness_function> experiment = std::make_shared<Experiment_executor_for_fitness_function>();
    experiment->to_get_model_to_run(acs_model.get());
    default_configuration_setter_obj.to_set_experiment_parameters(experiment.get());

    std::vector<double> records;
    experiment->to_set_vector(records);

    definder->to_set_signal(0);

    experiment->to_run();
    experiment->to_run();

    long double result = 0;
    //for (auto i : records) std::cout << i << '\t' << i * i << std::endl;
    for (auto i : records) result += i * i;
    std::cout << result << std::endl;
}

void to_check_registrator_to_std_vector()
{
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source> source = std::make_shared<DC_source>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());

    std::vector<double> records;
    std::shared_ptr<Experiment_executor_for_fitness_function> experiment = std::make_shared<Experiment_executor_for_fitness_function>();
    experiment->to_set_vector(records);
    experiment->to_get_model_to_run(acs_model.get());
    default_configuration_setter_obj.to_set_experiment_parameters(experiment.get());

    experiment->to_run();

    for(auto i : records) std::cout << i << std::endl;
}

void to_check_my_regulator_tunner_in_new_shell()
{
    // reference case
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());


    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_result_title("Before the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment);

    regulator->to_set_koefficients(5,5,5);
    definder->to_set_signal(20);
    experiment->to_run();


    // optimize
    parameters_for_optimizer parameters_for_optimizer_obj;
    default_configuration_setter_obj.to_set_configurations_in_parameters_for_optimizer(parameters_for_optimizer_obj);
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_acs_model = acs_model.get();
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_regulator = regulator.get();
    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Regulator_tuner_my_gradient_algorithm> optimizer = std::make_shared<Regulator_tuner_my_gradient_algorithm>(parameters_for_optimizer_obj);

    optimizer->to_tune();

    //return;

    std::array<double, 3> k = optimizer->to_check_answer();

    // optimized
    std::shared_ptr<Reference_signal_definder_static> definder_1 = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator_1 = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source_1 = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process_1 = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder_1,
                regulator_1,
                source_1,
                process_1
                );

    std::shared_ptr<Automated_control_system_paralleled> acs_model_1 = std::make_shared<Automated_control_system_paralleled>();
    acs_model_1->to_mount_the_element(definder_1.get());
    acs_model_1->to_mount_the_element(regulator_1.get());
    acs_model_1->to_mount_the_element(source_1.get());
    acs_model_1->to_mount_the_element(process_1.get());
    std::shared_ptr<Experiment_executor_short_report> experiment_1 = std::make_shared<Experiment_executor_short_report>();
    experiment_1->to_get_model_to_run(acs_model_1.get());
    experiment_1->to_set_result_title("After the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment_1);

    regulator_1->to_set_koefficients(k[0],k[1],k[2]);
    definder_1->to_set_signal(20);
    experiment_1->to_run();



    for (auto i : k) std::cout << i << '\t';
    std::cout << std::endl;
}

void to_check_my_regulator_tunner_automated_manual()
{
    // reference case
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());


    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_result_title("Before the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment);
    //set
    double load = 20;
    double inertia = 0.05;
    auto arr = process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

    arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
    process->to_set_all_parameters(arr);
    //alg
    regulator->to_set_koefficients(0.4);
    definder->to_set_signal(20);
    experiment->to_run();


    // optimize
    parameters_for_optimizer parameters_for_optimizer_obj;
    default_configuration_setter_obj.to_set_configurations_in_parameters_for_optimizer(parameters_for_optimizer_obj);
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_acs_model = acs_model.get();
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_regulator = regulator.get();
    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Regulator_tuner_my_ziegler_nichols_method> optimizer = std::make_shared<Regulator_tuner_my_ziegler_nichols_method>(parameters_for_optimizer_obj);

    optimizer->to_set_start_k(0.5);
    optimizer->to_tune();

    //return;

    std::array<double, 3> k = optimizer->to_check_answer();

    // optimized
    std::shared_ptr<Reference_signal_definder_static> definder_1 = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator_1 = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source_1 = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process_1 = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder_1,
                regulator_1,
                source_1,
                process_1
                );

    std::shared_ptr<Automated_control_system_paralleled> acs_model_1 = std::make_shared<Automated_control_system_paralleled>();
    acs_model_1->to_mount_the_element(definder_1.get());
    acs_model_1->to_mount_the_element(regulator_1.get());
    acs_model_1->to_mount_the_element(source_1.get());
    acs_model_1->to_mount_the_element(process_1.get());
    std::shared_ptr<Experiment_executor_short_report> experiment_1 = std::make_shared<Experiment_executor_short_report>();
    experiment_1->to_get_model_to_run(acs_model_1.get());
    experiment_1->to_set_result_title("After the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment_1);

    regulator_1->to_set_koefficients(k[0],k[1],k[2]);
    definder_1->to_set_signal(20);
    experiment_1->to_run();

    for (auto i : k) std::cout << i << '\t';
    std::cout << std::endl;
}

void to_run_reference_instance()
{
    //data
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());


    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_result_title("Before the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment);
    //set
    double load = 20;
    auto arr = process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;
    process->to_set_all_parameters(arr);
    //alg
    regulator->to_set_koefficients(5,5,5);
    definder->to_set_signal(20);
    experiment->to_run();
}
<<<<<<< HEAD
=======
void second_experiments()
{
    // reference case
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());


    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_result_title("Before the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment);
    //set
    double load = 20;
    double inertia = 0.05;
    auto arr = process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

    arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
    process->to_set_all_parameters(arr);
    //alg
    regulator->to_set_koefficients(10);
    definder->to_set_signal(20);
    //experiment->to_run();


    // optimize
    parameters_for_optimizer parameters_for_optimizer_obj;
    default_configuration_setter_obj.to_set_configurations_in_parameters_for_optimizer(parameters_for_optimizer_obj);
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_acs_model = acs_model.get();
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_regulator = regulator.get();
    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Regulator_tuner_my_gradient_algorithm> optimizer = std::make_shared<Regulator_tuner_my_gradient_algorithm>(parameters_for_optimizer_obj);

    optimizer->to_tune();

    //return;

    std::array<double, 3> k = optimizer->to_check_answer();

    // optimized
    std::shared_ptr<Reference_signal_definder_static> definder_1 = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator_1 = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source_1 = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process_1 = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder_1,
                regulator_1,
                source_1,
                process_1
                );

    std::shared_ptr<Automated_control_system_paralleled> acs_model_1 = std::make_shared<Automated_control_system_paralleled>();
    acs_model_1->to_mount_the_element(definder_1.get());
    acs_model_1->to_mount_the_element(regulator_1.get());
    acs_model_1->to_mount_the_element(source_1.get());
    acs_model_1->to_mount_the_element(process_1.get());
    std::shared_ptr<Experiment_executor_short_report> experiment_1 = std::make_shared<Experiment_executor_short_report>();
    experiment_1->to_get_model_to_run(acs_model_1.get());
    experiment_1->to_set_result_title("After the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment_1);

    regulator_1->to_set_koefficients( k[0],	k[1], k[2] );  //there
    definder_1->to_set_signal(20);
    experiment_1->to_set_t_length(720);
    experiment_1->to_run();

    for (auto i : k) std::cout << i << '\t';
    std::cout << std::endl;
}
void first_and_second_experiments()
{
    // reference case
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());


    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_result_title("Before the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment);
    //set
    double load = 20;
    double inertia = 0.05;
    auto arr = process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

    arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
    process->to_set_all_parameters(arr);
    //alg
    regulator->to_set_koefficients(0);
    definder->to_set_signal(20);
    //experiment->to_run();


    // optimize
    parameters_for_optimizer parameters_for_optimizer_obj;
    default_configuration_setter_obj.to_set_configurations_in_parameters_for_optimizer(parameters_for_optimizer_obj);
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_acs_model = acs_model.get();
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_regulator = regulator.get();
    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Regulator_tuner_my_ziegler_nichols_method> optimizer_0 = std::make_shared<Regulator_tuner_my_ziegler_nichols_method>(parameters_for_optimizer_obj);
    optimizer_0->to_set_start_k(0.5);
    optimizer_0->to_tune();

    std::shared_ptr<Regulator_tuner_my_gradient_algorithm> optimizer_1 = std::make_shared<Regulator_tuner_my_gradient_algorithm>(parameters_for_optimizer_obj);
    optimizer_1->to_tune();

    //return;

    std::array<double, 3> k = optimizer_1->to_check_answer();

    // optimized
    std::shared_ptr<Reference_signal_definder_static> definder_1 = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator_1 = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source_1 = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process_1 = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder_1,
                regulator_1,
                source_1,
                process_1
                );

    std::shared_ptr<Automated_control_system_paralleled> acs_model_1 = std::make_shared<Automated_control_system_paralleled>();
    acs_model_1->to_mount_the_element(definder_1.get());
    acs_model_1->to_mount_the_element(regulator_1.get());
    acs_model_1->to_mount_the_element(source_1.get());
    acs_model_1->to_mount_the_element(process_1.get());
    std::shared_ptr<Experiment_executor_short_report> experiment_1 = std::make_shared<Experiment_executor_short_report>();
    experiment_1->to_get_model_to_run(acs_model_1.get());
    experiment_1->to_set_result_title("After the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment_1);

    regulator_1->to_set_koefficients( k[0],	k[1], k[2] );  //there
    definder_1->to_set_signal(20);
    experiment_1->to_set_t_length(720);
    experiment_1->to_run();

    for (auto i : k) std::cout << i << '\t';
    std::cout << std::endl;
}

void third_experiments()
{
    // reference case
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());


    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_result_title("Before the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment);
    //set
    double load = 20;
    double inertia = 0.05;
    auto arr = process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

    arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
    process->to_set_all_parameters(arr);
    //alg
    regulator->to_set_koefficients(10);
    definder->to_set_signal(20);
    //experiment->to_run();


    // optimize
    parameters_for_optimizer parameters_for_optimizer_obj;
    default_configuration_setter_obj.to_set_configurations_in_parameters_for_optimizer(parameters_for_optimizer_obj);
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_acs_model = acs_model.get();
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_regulator = regulator.get();
    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Regulator_tuner_my_generative_algorithm> optimizer = std::make_shared<Regulator_tuner_my_generative_algorithm>(parameters_for_optimizer_obj);

    optimizer->to_tune();

    //return;

    std::array<double, 3> k = optimizer->to_check_answer();

    // optimized
    std::shared_ptr<Reference_signal_definder_static> definder_1 = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator_1 = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source_1 = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process_1 = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder_1,
                regulator_1,
                source_1,
                process_1
                );

    std::shared_ptr<Automated_control_system_paralleled> acs_model_1 = std::make_shared<Automated_control_system_paralleled>();
    acs_model_1->to_mount_the_element(definder_1.get());
    acs_model_1->to_mount_the_element(regulator_1.get());
    acs_model_1->to_mount_the_element(source_1.get());
    acs_model_1->to_mount_the_element(process_1.get());
    std::shared_ptr<Experiment_executor_short_report> experiment_1 = std::make_shared<Experiment_executor_short_report>();
    experiment_1->to_get_model_to_run(acs_model_1.get());
    experiment_1->to_set_result_title("After the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment_1);

    regulator_1->to_set_koefficients( k[0],	k[1], k[2] );  //there
    definder_1->to_set_signal(20);
    experiment_1->to_set_t_length(720);
    experiment_1->to_run();

    for (auto i : k) std::cout << i << '\t';
    std::cout << std::endl;
}

#include <thread>

void to_mantain_acs_model_for_experiments(parameters_for_optimizer** p_p_parameters_for_optimizer_obj, int* p_threads_signal)
{
    while (1)
    {

        // As an ordinar case
        std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
        std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
        std::shared_ptr<DC_source_inerted> source = std::make_shared<DC_source_inerted>();
        std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

        default_configuration_setter_obj.to_set_elements_parameters(
                    definder,
                    regulator,
                    source,
                    process
                    );

        process->to_set_calculation_mode(DC_engine::EULER);

        std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
        acs_model->to_mount_the_element(definder.get());
        acs_model->to_mount_the_element(regulator.get());
        acs_model->to_mount_the_element(source.get());
        acs_model->to_mount_the_element(process.get());


        std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
        experiment->to_get_model_to_run(acs_model.get());
        experiment->to_set_result_title("Before the regulator tuning");
        default_configuration_setter_obj.to_set_experiment_parameters(experiment);
        //set
        double load = 20;
        double inertia = 0.05;
        auto arr = process->to_get_parameters();
        arr[DC_engine::LOAD_K_0] = 1 * load;
        arr[DC_engine::LOAD_K_1] = 0 * load;
        arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
        arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

        arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
        process->to_set_all_parameters(arr);
        //alg
        regulator->to_set_koefficients(10);
        definder->to_set_signal(20);

        ///
        /// ATTENTION!
        *p_p_parameters_for_optimizer_obj = new parameters_for_optimizer;
        parameters_for_optimizer& parameters_for_optimizer_obj = **p_p_parameters_for_optimizer_obj;
        /// ATTENTION!
        ///

        // user parameters set
        default_configuration_setter_obj.to_set_configurations_in_parameters_for_optimizer(parameters_for_optimizer_obj);
        parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_acs_model = acs_model.get();
        parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_regulator = regulator.get();
        process->to_set_calculation_mode(DC_engine::EULER);

        /// START OF EVALUATION
        *p_threads_signal = 1;
        while (*p_threads_signal > 0) continue;
        /// END OF EVALUATION
        delete *p_p_parameters_for_optimizer_obj;
        *p_p_parameters_for_optimizer_obj = nullptr;
        if (*p_threads_signal == 0) break;
    }
    while (1)
    {

        // As an ordinar case
        std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
        std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
        std::shared_ptr<DC_source_inerted> source = std::make_shared<DC_source_inerted>();
        std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

        default_configuration_setter_obj.to_set_elements_parameters(
                    definder,
                    regulator,
                    source,
                    process
                    );

        process->to_set_calculation_mode(DC_engine::EULER);

        std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
        acs_model->to_mount_the_element(definder.get());
        acs_model->to_mount_the_element(regulator.get());
        acs_model->to_mount_the_element(source.get());
        acs_model->to_mount_the_element(process.get());


        std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
        experiment->to_get_model_to_run(acs_model.get());
        experiment->to_set_result_title("Before the regulator tuning");
        default_configuration_setter_obj.to_set_experiment_parameters(experiment);
        //set
        double load = 20;
        double inertia = 0.05;
        auto arr = process->to_get_parameters();
        arr[DC_engine::LOAD_K_0] = 0.5 * load;
        arr[DC_engine::LOAD_K_1] = 0.2 * load;
        arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
        arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

        arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
        process->to_set_all_parameters(arr);
        //alg
        regulator->to_set_koefficients(10);
        definder->to_set_signal(20);

        ///
        /// ATTENTION!
        *p_p_parameters_for_optimizer_obj = new parameters_for_optimizer;
        parameters_for_optimizer& parameters_for_optimizer_obj = **p_p_parameters_for_optimizer_obj;
        /// ATTENTION!
        ///

        // user parameters set
        default_configuration_setter_obj.to_set_configurations_in_parameters_for_optimizer(parameters_for_optimizer_obj);
        parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_acs_model = acs_model.get();
        parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_regulator = regulator.get();
        process->to_set_calculation_mode(DC_engine::EULER);

        /// START OF EVALUATION
        *p_threads_signal = 1;
        while (*p_threads_signal > 0) continue;
        /// END OF EVALUATION
        delete *p_p_parameters_for_optimizer_obj;
        *p_p_parameters_for_optimizer_obj = nullptr;
        if (*p_threads_signal == 0) break;
    }
    *p_threads_signal = 1;
}

void param_set(parameters_for_optimizer** param, int step)
{
    parameters_for_optimizer& parameters_for_optimizer_obj = **param;
    switch (step)
    {
    case 11:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 20;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 20;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 360;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
        break;
    case 12:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 50;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 50;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 360;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
        break;
    case 13:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 100;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 100;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 360;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
        break;
    case 14:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 100;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 50;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 180;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 2;
        break;
    case 21:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 20;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 20;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 720;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
        break;
    case 22:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 50;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 50;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 720;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
        break;
    case 23:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 100;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 100;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 720;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
        break;
    case 24:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 100;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 50;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 360;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 2;
        break;
    case 31:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 20;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 20;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 1440;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
        break;
    case 32:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 50;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 50;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 1440;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
        break;
    case 33:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 100;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 100;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 1440;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
        break;
    case 34:
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 100;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 50;
        parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 720;
        parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 2;
        break;
    default:
        std::cerr << "ERROR param_set()" << std::endl << std::flush;
        break;
    }
}
void to_evaluate_put_in(std::shared_ptr<std::multimap<long double, std::pair<const char*, std::array<double,3>>>>, parameters_for_optimizer**, int*, int);
void to_show_results(const std::multimap<long double, std::pair<const char*, std::array<double,3>>>&);
void to_set_results_to_evaluate(std::multimap<long double, std::pair<const char*, std::array<double,3>>> & arr)
{
    arr.emplace( std::make_pair( 0 , std::make_pair( "Gradient method '1,0,0' init" , std::array<double,3> { 1.06758, 0.317347, 0.579046 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Gradient method '2,0,0' init" , std::array<double,3> { 8.38471, -29403.1, -341273 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Gradient method '5,0,0' init" , std::array<double,3> { 6.13157, 1.49139, 1.30597 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Gradient method '7,0,0' init" , std::array<double,3> { 7.69889, 0.876194, 0.881272 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Gradient method '10,0,0' init" , std::array<double,3> { 10.474, 0.568234, 0.571808 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Zeingler-Nichols method 'x0.5 and x2'+'x0.33 and x3' at '0.5 step k_crit' to avrg pass" , std::array<double,3> { 0.175, 0.0190217, 1.0626 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Zeingler-Nichols method 0.1 step k_crit" , std::array<double,3> { 0.595, 0.111215, 2.10095 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Zeingler-Nichols method 1 step k_crit" , std::array<double,3> { 0.8, 0.137339, 3.0756 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Generative method non mutationable a lot #1" , std::array<double,3> { 3.9, 3.5, 5.3	} ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Generative method non mutationable a lot #2" , std::array<double,3> { 5.1, 2.2, 6.7 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Generative method non mutationable a lot #3" , std::array<double,3> { 5.2, 1.2, 2.8 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Generative method non mutationable a lot #4" , std::array<double,3> { 5.8, 6.4, 7.2 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Generative method non mutationable a lot #5" , std::array<double,3> { 4, 4.3, 0.1 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Generative method mutationable #1" , std::array<double,3> { 9.4, 7, 3.5 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Generative method mutationable #2" , std::array<double,3> { 9.1, 5, 0 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Generative method mutationable #3" , std::array<double,3> { 8.8, 2.5, 5.2 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Generative method mutationable #4" , std::array<double,3> { 1.7, 0.8, 0.3 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Generative method mutationable #5" , std::array<double,3> { 0.5, 6.7, 1.3 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Gradient method with init by the Zeingler-Nichols method 0.5 step k_krit " , std::array<double,3> { 5.7759, 5.34597, 7.63554 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Gradient method with init by the Zeingler-Nichols method 0.1 step k_krit " , std::array<double,3> { 4.70099, 4.2221, 6.21183 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "Gradient method with init by the Zeingler-Nichols method 1 step k_krit " , std::array<double,3> { 2.49195, 1.82929, 4.76755 } ) ) );
}
void evaluate_resuls()
{
    //      setting arr of the experiments data:
    std::shared_ptr<std::multimap<long double, std::pair<const char*, std::array<double,3>>>> arr_to_evaluate
            = std::make_shared<std::multimap<long double, std::pair<const char*, std::array<double,3>>>>();
    to_set_results_to_evaluate(*arr_to_evaluate);


    //      setting equipment for the experimental evaluation
    int* p_threads_signal = new int(0);
    parameters_for_optimizer ** p_p_parameters_for_optimizer_obj = new (parameters_for_optimizer*);
    *p_p_parameters_for_optimizer_obj = nullptr;
    std::thread thread_acs_equipment(to_mantain_acs_model_for_experiments, p_p_parameters_for_optimizer_obj, p_threads_signal);
    thread_acs_equipment.detach();


    //parameters_for_optimizer& parameters_for_optimizer_obj = *p_parameters_for_optimizer_obj;


    // Evaluation process:

    //      Non mutable reference signal the tuning have been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 11);
    to_show_results(*arr_to_evaluate);

    //      Non mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 12);
    to_show_results(*arr_to_evaluate);

    //      Another non mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 13);
    to_show_results(*arr_to_evaluate);

    //      Mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 14);
    to_show_results(*arr_to_evaluate);

    // Longer:
    //      Non mutable reference signal the tuning have been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 21);
    to_show_results(*arr_to_evaluate);

    //      Non mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 22);
    to_show_results(*arr_to_evaluate);

    //      Another non mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 23);
    to_show_results(*arr_to_evaluate);

    //      Mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 24);
    to_show_results(*arr_to_evaluate);

    // Longest:
    //      Non mutable reference signal the tuning have been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 31);
    to_show_results(*arr_to_evaluate);

    //      Non mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal,32);
    to_show_results(*arr_to_evaluate);

    //      Another non mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 33);
    to_show_results(*arr_to_evaluate);

    //      Mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 34);
    to_show_results(*arr_to_evaluate);



    // Long and mutable torque depend upon the rotation velocity:
    *p_threads_signal = 0;
    //      Non mutable reference signal the tuning have been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 21);
    to_show_results(*arr_to_evaluate);

    //      Non mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 22);
    to_show_results(*arr_to_evaluate);

    //      Another non mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 23);
    to_show_results(*arr_to_evaluate);

    //      Mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 24);
    to_show_results(*arr_to_evaluate);

    // Short and mutable torque depend upon the rotation velocity:
    *p_threads_signal = 0;
    //      Non mutable reference signal the tuning have been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 11);
    to_show_results(*arr_to_evaluate);

    //      Non mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 12);
    to_show_results(*arr_to_evaluate);

    //      Another non mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 13);
    to_show_results(*arr_to_evaluate);

    //      Mutable reference signal the tuning have NOT been made for
    to_evaluate_put_in(arr_to_evaluate, p_p_parameters_for_optimizer_obj, p_threads_signal, 14);
    to_show_results(*arr_to_evaluate);

    *p_threads_signal = 0;
    while (*p_threads_signal < 1) continue;
    delete p_threads_signal;
}
void to_evaluate_put_in(std::shared_ptr<std::multimap<long double, std::pair<const char*, std::array<double,3>>>> arr, parameters_for_optimizer** param, int* p_threads_signal, int step)
{
    std::shared_ptr<std::multimap<long double, std::pair<const char*, std::array<double,3>>>> arr_to_rate
            = std::make_shared<std::multimap<long double, std::pair<const char*, std::array<double,3>>>>();
    unsigned int num = 0;
    std::cout << arr->size() << ":\t" << std::flush;
    for (auto i : *arr)
    {
        std::cout << ' ' << num << std::flush;

        ///
        /// START OF EVALUATION
        while (*p_threads_signal < 1) continue;
        param_set(param, step);
        arr_to_rate->emplace(
                                fitness_function_varied_reference_signal(i.second.second.begin(), *param),
                                i.second
                            );
       *p_threads_signal = -1;
       while (*p_threads_signal < 1) continue;
       /// END OF EVALUATION
       ///

       ++num;
    }
    arr->swap(*arr_to_rate);
    arr_to_rate->clear();
    std::cout << "\tDone!\n" << std::flush;
}
void to_show_results(const std::multimap<long double, std::pair<const char*, std::array<double,3>>>& arr)
{
    std::ofstream fout("Evaluation_report.txt", std::ios::app);
    unsigned int n = 1;
    static unsigned int n_glob = 0;
    ++n_glob;
    std::cout << "Results #" << n_glob << ":\n" << std::flush;
    fout << "Results #" << n_glob << ":\n" << std::flush;
    for (const auto i : arr)
    {
        std::cout << n << ":\t\t" << i.first << "\t\t" << i.second.first << "\t\t" << std::flush;
        fout << n << ":\t\t" << i.first << "\t\t" << i.second.first << "\t\t" << std::flush;
        for (auto j : i.second.second)
        {
            std::cout << j << "\t\t" << std::flush;
            fout << j << "\t\t" << std::flush;
        }
        std::cout << std::endl;
        fout << std::endl;
        ++n;
    }
    std::cout << "The end of rating list\n\n" << std::endl << std::flush;
    fout << "The end of rating list\n\n" << std::endl << std::flush;
}

>>>>>>> 47396ac (Report for the diploma project has been made successfully.)
