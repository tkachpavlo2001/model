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

int main()
{
    cout << "Hello World!" << endl;
    //to_pid_regulate_3();
    to_pid_regulate_4();
    to_check_fitness_f();
    //to_check_registrator_to_std_vector();
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

void to_check_fitness_f()
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

    std::shared_ptr<Regulator_tuner> tuner = std::make_shared<Regulator_tuner>();
    tuner->to_set_model_and_regulator(acs_model, regulator);
    //(Regulator_tuner_iterface* _tuner, double _dt, double _length, double _t_registrate, double _min, double _max)
    std::cout << fitness_function_varied_reference_signal(tuner.get(), 1e-5, 10, 1e-1, 2, 0, 0) << std::endl ;
}
