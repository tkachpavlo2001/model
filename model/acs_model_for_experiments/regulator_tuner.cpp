#include "regulator_tuner.h"

#include <gsl/gsl_multimin.h>

Regulator_tuner_iterface::Regulator_tuner_iterface(){}

Regulator_tuner_iterface::~Regulator_tuner_iterface(){}

void Regulator_tuner_iterface::to_set_model_and_regulator(std::shared_ptr<Automated_control_system> _m, std::shared_ptr<PID_regulator> _c)
{
    acs_model = _m;
    controller = _c;
}

Regulator_tuner::Regulator_tuner()
{

}
Regulator_tuner::~Regulator_tuner()
{

}
