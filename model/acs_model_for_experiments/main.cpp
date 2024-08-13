#include <iostream>
#include <memory>
#include "automated_control_system.h"
#include "experiment_executor.h"
#include "default_configuration_setter.h"

#include <cmath>

using namespace std;

const Default_configuration_setter default_configuration_setter_obj;

void to_pid_regulate_3();
void to_check_registrator_to_std_vector();

int main()
{
    cout << "Hello World!" << endl;
    //to_pid_regulate_3();
    to_check_registrator_to_std_vector();
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
