#include <iostream>
#include <memory>
#include "automated_control_system.h"
#include "experiment_executor.h"
#include "default_configuration_setter.h"
#include "regulator_tuner.h"

#include <cmath>

using namespace std;

const Default_configuration_setter default_configuration_setter_obj;

void first_experiments_auto();
void first_experiments();
void second_experiments();
void first_and_second_experiments();
void third_experiments();
void evaluate_resuls();

int main()
{
    evaluate_resuls();
    return 0;
}
void first_experiments_auto()
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
    regulator->to_set_koefficients(1);
    definder->to_set_signal(20);
    //experiment->to_run();


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

    regulator_1->to_set_koefficients( 10 * 0.2625,	0.5 * 0.0272021,	10 * 1.67186	);  //there
    definder_1->to_set_signal(20);
    experiment_1->to_set_t_length(720);
    experiment_1->to_run();

    for (auto i : k) std::cout << i << '\t';
    std::cout << std::endl;
}
void first_experiments()
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
    regulator->to_set_koefficients(1);
    definder->to_set_signal(20);
    //experiment->to_run();


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

    regulator_1->to_set_koefficients( 10 * 0.2625,	0.5 * 0.0272021,	10 * 1.67186	);  //there
    definder_1->to_set_signal(20);
    experiment_1->to_set_t_length(720);
    experiment_1->to_run();

    for (auto i : k) std::cout << i << '\t';
    std::cout << std::endl;
}
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
    optimizer_0->to_set_start_k(1);
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



void to_evaluate_put_in(std::shared_ptr<std::multimap<long double, std::pair<const char*, std::array<double,3>>>>, parameters_for_optimizer*);
void to_show_results(const std::multimap<long double, std::pair<const char*, std::array<double,3>>>&);
void to_set_results_to_evaluate(std::multimap<long double, std::pair<const char*, std::array<double,3>>> & arr)
{
    arr.emplace( std::make_pair( 0 , std::make_pair( "0" , std::array<double,3> { 0, 0, 0 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "0" , std::array<double,3> { 0, 0, 0 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "0" , std::array<double,3> { 0, 0, 0 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "0" , std::array<double,3> { 0, 0, 0 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "0" , std::array<double,3> { 0, 0, 0 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "0" , std::array<double,3> { 0, 0, 0 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "0" , std::array<double,3> { 0, 0, 0 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "0" , std::array<double,3> { 0, 0, 0 } ) ) );
    arr.emplace( std::make_pair( 0 , std::make_pair( "0" , std::array<double,3> { 0, 0, 0 } ) ) );
}
void evaluate_resuls()
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


    // Evaluation part

    //      setting arr:
    std::shared_ptr<std::multimap<long double, std::pair<const char*, std::array<double,3>>>> arr_to_evaluate
            = std::make_shared<std::multimap<long double, std::pair<const char*, std::array<double,3>>>>();
    to_set_results_to_evaluate(*arr_to_evaluate);
    //      setting metrics:
    //      from optimize
    parameters_for_optimizer parameters_for_optimizer_obj;
    default_configuration_setter_obj.to_set_configurations_in_parameters_for_optimizer(parameters_for_optimizer_obj);
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_acs_model = acs_model.get();
    parameters_for_optimizer_obj.parameters_p_objects_parameters_obj.p_regulator = regulator.get();
    process->to_set_calculation_mode(DC_engine::EULER);

    // Evaluation process:

    //      Non mutable reference signal the tuning have been made for
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 20;
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 20;
    parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 360;
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
    to_evaluate_put_in(arr_to_evaluate, &parameters_for_optimizer_obj);
    to_show_results(*arr_to_evaluate);

    //      Non mutable reference signal the tuning have NOT been made for
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 50;
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 50;
    parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 360;
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
    to_evaluate_put_in(arr_to_evaluate, &parameters_for_optimizer_obj);
    to_show_results(*arr_to_evaluate);

    //      Another non mutable reference signal the tuning have NOT been made for
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 100;
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 100;
    parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 360;
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 1;
    to_evaluate_put_in(arr_to_evaluate, &parameters_for_optimizer_obj);
    to_show_results(*arr_to_evaluate);

    //      Mutable reference signal the tuning have NOT been made for
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.min = 100;
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.max = 50;
    parameters_for_optimizer_obj.parameters_for_fitness_function_obj.length = 180;
    parameters_for_optimizer_obj.parameters_for_varied_fitness_function_obj.times = 2;
    to_evaluate_put_in(arr_to_evaluate, &parameters_for_optimizer_obj);
    to_show_results(*arr_to_evaluate);
}

void to_evaluate_put_in(std::shared_ptr<std::multimap<long double, std::pair<const char*, std::array<double,3>>>> arr, parameters_for_optimizer* param)
{
    std::shared_ptr<std::multimap<long double, std::pair<const char*, std::array<double,3>>>> arr_to_rate
            = std::make_shared<std::multimap<long double, std::pair<const char*, std::array<double,3>>>>();

    unsigned int num = 0;
    std::cout << arr->size() << ": " << num;
    for (auto i : *arr)
    {
        std::cout << ' ' << num;
        arr_to_rate->emplace(
                                fitness_function_varied_reference_signal(i.second.second.begin(), param),
                                i.second
                            );
    }
    arr->swap(*arr_to_rate);
    arr_to_rate->clear();
    std::cout << "Done!\n";
}

void to_show_results(const std::multimap<long double, std::pair<const char*, std::array<double,3>>>& arr)
{
    unsigned int n = 0;
    std::cout << "Results:\n";
    for (const auto i : arr)
    {
        std::cout << n << ": " << i.first << "\t" << i.second.first << "\t";
        for (auto j : i.second.second)
        {
            std::cout << j;
        }
        std::cout << std::endl;
    }
    std::cout << "The end of rating list\n" << std::endl;
}

