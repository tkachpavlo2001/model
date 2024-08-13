#include "regulator_tuner.h"
#include "experiment_executor.h"


#include <memory.h>
#include <vector>
#include <gsl/gsl_multimin.h>
#include <numeric>

double fitness_function_varied_reference_signal
(Regulator_tuner_iterface* _tuner, Automated_control_system * _acs_model, double _dt, double _length, double _t_registrate, double _min, double _max)
{
    std::shared_ptr<Experiment_executor_for_fitness_function_with_varied_reference_signal> p_experiment =
            std::make_shared<Experiment_executor_for_fitness_function_with_varied_reference_signal>(_tuner->to_get_model());
    p_experiment->to_set_varied_diapasone_min_max(_min, _max);
    p_experiment->to_set_dt(_dt);
    p_experiment->to_set_t_length(_length);
    p_experiment->to_set_time_to_registrate(_t_registrate);

    std::vector<double> records;
    p_experiment->to_set_vector(records);

    p_experiment->to_get_model_to_run(_acs_model);

    p_experiment->to_run();
    p_experiment->to_run();

    return std::accumulate(records.begin(), records.end(), 0, [&](double acc, double num)->double {return acc + num * num;} );
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

