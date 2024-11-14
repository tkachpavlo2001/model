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
