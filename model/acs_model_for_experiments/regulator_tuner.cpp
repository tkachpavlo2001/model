#include "regulator_tuner.h"
#include "experiment_executor.h"


#include <memory.h>
#include <vector>
#include <gsl/gsl_multimin.h>

Regulator_tuner_iterface::Regulator_tuner_iterface(){}

Regulator_tuner_iterface::~Regulator_tuner_iterface(){}

void Regulator_tuner_iterface::to_set_model_and_regulator(Automated_control_system * _m, PID_regulator * _c)
{
    acs_model = _m;
    controller = _c;
}
void Regulator_tuner_iterface::to_set_model_and_regulator(std::shared_ptr<Automated_control_system> _m, std::shared_ptr<PID_regulator> _c)
{
    to_set_model_and_regulator(_m.get(), _c.get());
}
Regulator_tuner::Regulator_tuner()
{

}
Regulator_tuner::~Regulator_tuner()
{

}

double fitness_function(Regulator_tuner_iterface&)
{
    std::shared_ptr<Experiment_executor_for_fitness_function> experiment;
}
