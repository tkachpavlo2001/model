#include "regulator_tuner.h"
#include "experiment_executor.h"


#include <memory.h>
#include <vector>
#include <gsl/gsl_multimin.h>

double fitness_function_varied_reference_signal(Regulator_tuner_iterface* _tuner)
{
    std::shared_ptr<Experiment_executor_for_fitness_function> p_experiment = std::make_shared<Experiment_executor_for_fitness_function>(_tuner->to_get_model());
    std::vector<double> results;
}

Regulator_tuner_iterface::Regulator_tuner_iterface(){}

Regulator_tuner_iterface::~Regulator_tuner_iterface(){}

void Regulator_tuner_iterface::to_set_model_and_regulator(Automated_control_system * _m, PID_regulator * _c)
{
    acs_model = _m;
    regulator = _c;
}
void Regulator_tuner_iterface::to_set_model_and_regulator(std::shared_ptr<Automated_control_system> _m, std::shared_ptr<PID_regulator> _c)
{
    to_set_model_and_regulator(_m.get(), _c.get());
}
Automated_control_system * Regulator_tuner_iterface::to_get_model()
{
    return acs_model;
}
PID_regulator * Regulator_tuner_iterface::to_get_regulator()
{
    return regulator;
}
Regulator_tuner::Regulator_tuner()
{

}
Regulator_tuner::~Regulator_tuner()
{

}

