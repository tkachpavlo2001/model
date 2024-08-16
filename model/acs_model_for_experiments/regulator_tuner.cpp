#include "regulator_tuner.h"
#include "experiment_executor.h"


#include <memory.h>
#include <vector>
#include <gsl/gsl_multimin.h>
#include <numeric>

long double fitness_function_varied_reference_signal
(Regulator_tuner_iterface* _tuner, double _dt, double _length, double _t_registrate, double _times, double _min, double _max)
{
    std::shared_ptr<Experiment_executor_for_fitness_function_with_varied_reference_signal> p_experiment =
            std::make_shared<Experiment_executor_for_fitness_function_with_varied_reference_signal>(_tuner->to_get_model());
    p_experiment->to_set_varied_diapasone_min_max(_min, _max);
    p_experiment->to_set_dt(_dt);
    p_experiment->to_set_t_length(_length);
    p_experiment->to_set_time_to_registrate(_t_registrate);

    std::vector<double> records;
    p_experiment->to_set_vector(records);

    p_experiment->to_get_model_to_run(p_experiment->to_get_model());

    for (int i = 0; i < _times; ++i) p_experiment->to_run();

    return std::accumulate(records.begin(), records.end(), 0, [&](double acc, double num)->double {return acc + num * num;} );
}
long double fitness_function_varied_reference_signal(user_parameters_for_gsl_optimizer* p_my_parameters)
{
    return fitness_function_varied_reference_signal(
        p_my_parameters->p_tuner,
        p_my_parameters->dt,
        p_my_parameters->length,
        p_my_parameters->t_registrate,
        p_my_parameters->times,
        p_my_parameters->min,
        p_my_parameters->max
        );
}
double my_function(const gsl_vector * _p_vector, void * _p_user_parameters)
{
    user_parameters_for_gsl_optimizer * p_my_parameters = (user_parameters_for_gsl_optimizer *) _p_user_parameters;
    p_my_parameters->p_regulator->to_set_koefficients(
                gsl_vector_get(_p_vector,0),
                gsl_vector_get(_p_vector,1),
                gsl_vector_get(_p_vector,2)
                );
    return fitness_function_varied_reference_signal(p_my_parameters);
}
void my_function_gradient(const gsl_vector * _p_vector, void * _p_user_parameters, gsl_vector * _p_gradient_vector)
{
    user_parameters_for_gsl_optimizer * p_my_parameters = (user_parameters_for_gsl_optimizer *) _p_user_parameters;
    double previous_value;

    previous_value = p_my_parameters->p_regulator->to_check_parameters()[PID_regulator::K_P];
    p_my_parameters->p_regulator->to_get_parameters()[PID_regulator::K_P] += p_my_parameters->h;
    gsl_vector_set(_p_gradient_vector, 0, fitness_function_varied_reference_signal(p_my_parameters) - p_my_parameters->last_value_f);
    p_my_parameters->p_regulator->to_get_parameters()[PID_regulator::K_P] = previous_value;

    previous_value = p_my_parameters->p_regulator->to_check_parameters()[PID_regulator::K_I];
    p_my_parameters->p_regulator->to_get_parameters()[PID_regulator::K_I] += p_my_parameters->h;
    gsl_vector_set(_p_gradient_vector, 0, fitness_function_varied_reference_signal(p_my_parameters) - p_my_parameters->last_value_f);
    p_my_parameters->p_regulator->to_get_parameters()[PID_regulator::K_I] = previous_value;

    previous_value = p_my_parameters->p_regulator->to_check_parameters()[PID_regulator::K_D];
    p_my_parameters->p_regulator->to_get_parameters()[PID_regulator::K_D] += p_my_parameters->h;
    gsl_vector_set(_p_gradient_vector, 0, fitness_function_varied_reference_signal(p_my_parameters) - p_my_parameters->last_value_f);
    p_my_parameters->p_regulator->to_get_parameters()[PID_regulator::K_D] = previous_value;
}
void my_functions_f_and_gradient(const gsl_vector * _p_vector, void * _p_user_parameters, double * f, gsl_vector* _p_gradient_vector)
{
    user_parameters_for_gsl_optimizer * p_my_parameters = (user_parameters_for_gsl_optimizer *) _p_user_parameters;
    *f = p_my_parameters->last_value_f = my_function(_p_vector, _p_user_parameters);
    my_function_gradient(_p_vector, _p_user_parameters, _p_gradient_vector);
}

Regulator_tuner_iterface::Regulator_tuner_iterface(Automated_control_system * _m, PID_regulator * _r)
{
    to_set_model_and_regulator(_m, _r);
}
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

Regulator_tuner_gradient_method::Regulator_tuner_gradient_method(Automated_control_system * _m, PID_regulator * _r) : Regulator_tuner_iterface(_m, _r)
{

}
Regulator_tuner_gradient_method::~Regulator_tuner_gradient_method()
{}

Regulator_tuner::Regulator_tuner()
{

}
Regulator_tuner::~Regulator_tuner()
{

}

