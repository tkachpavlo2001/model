#ifndef DEFAULT_CONFIGURATION_SETTER_H
#define DEFAULT_CONFIGURATION_SETTER_H

#include"controlled_process.h"
#include "automated_control_system.h"
#include "experiment_executor.h"

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
};

#endif // DEFAULT_CONFIGURATION_SETTER_H