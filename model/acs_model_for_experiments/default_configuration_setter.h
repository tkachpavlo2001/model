/*
 * This file is part of ACS_MODEL.
 *
 * ACS_MODEL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACS_MODEL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACS_MODEL. If not, see <https://www.gnu.org/licenses/>.
 */

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
    void to_set_experiment_parameters(std::shared_ptr<Experiment_executor_interface>) const;
    void to_set_experiment_parameters(Experiment_executor_interface*) const;
    //void to_set_configurations_in_user_parameters_for_gsl_optimizer(user_parameters_for_gsl_optimizer*) const;
    //void to_set_objects_in_user_parameters_for_gsl_optimizer(user_parameters_for_gsl_optimizer*, Regulator_tuner_interface*,Automated_control_system*,PID_regulator*) const;
    //void to_set_configurations_in_my_tuner(std::shared_ptr<Regulator_tuner_my_generic_algorithm>) const;
    void to_set_configurations_in_parameters_for_optimizer(parameters_for_optimizer& _arg) const;
};

#endif // DEFAULT_CONFIGURATION_SETTER_H
