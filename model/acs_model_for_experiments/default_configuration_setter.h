#ifndef DEFAULT_CONFIGURATION_SETTER_H
#define DEFAULT_CONFIGURATION_SETTER_H

#include"controlled_process.h"
#include "automated_control_system.h"
#include "experiment_executor.h"
#include "regulator_tuner.h"

#include<memory>

class Default_configuration_setter
{
public:
    Default_configuration_setter();
    void to_fill_process(std::shared_ptr<DC_engine> drive) const;
    void to_fill_process(DC_engine* drive) const;
    void to_set_elements_parameters(std::shared_ptr<Reference_signal_definder_static>,std::shared_ptr<PID_regulator>,std::shared_ptr<DC_source>,std::shared_ptr<DC_engine>) const;
    void to_set_elements_parameters(Reference_signal_definder_static*,PID_regulator*,DC_source*,DC_engine*) const;
    void to_set_experiment_parameters(std::shared_ptr<Experiment_executor>) const;
    void to_set_experiment_parameters(Experiment_executor_interface*) const;
    //void to_set_configurations_in_user_parameters_for_gsl_optimizer(user_parameters_for_gsl_optimizer*) const;
    //void to_set_objects_in_user_parameters_for_gsl_optimizer(user_parameters_for_gsl_optimizer*, Regulator_tuner_interface*,Automated_control_system*,PID_regulator*) const;
    //void to_set_configurations_in_my_tuner(std::shared_ptr<Regulator_tuner_my_generic_algorithm>) const;
    void to_set_configurations_in_parameters_for_optimizer(parameters_for_optimizer& _arg) const;
};

#endif // DEFAULT_CONFIGURATION_SETTER_H
