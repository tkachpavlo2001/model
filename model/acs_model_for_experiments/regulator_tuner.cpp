#include "regulator_tuner.h"
#include "experiment_executor.h"


#include <memory.h>
#include <vector>
#include <gsl/gsl_multimin.h>
#include <numeric>

long double fitness_function_varied_reference_signal
(Regulator_tuner_interface* _tuner, double _dt, double _length, double _t_registrate, double _times, double _min, double _max)
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

double my_function(const gsl_vector *, void *);

gsl_vector * v = gsl_vector_alloc(3);
double my_function_for_mine(double * n, void * _p_user_parameters)
{
    gsl_vector_set(v, 0, n[0]);
    gsl_vector_set(v, 1, n[1]);
    gsl_vector_set(v, 2, n[2]);
    return my_function(v, _p_user_parameters);
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
    gsl_vector_set(_p_gradient_vector, 0, fitness_function_varied_reference_signal(p_my_parameters) - p_my_parameters->last_value_f);/// (,?????,)
    p_my_parameters->p_regulator->to_get_parameters()[PID_regulator::K_D] = previous_value;
}
void my_functions_f_and_gradient(const gsl_vector * _p_vector, void * _p_user_parameters, double * f, gsl_vector* _p_gradient_vector)
{
    user_parameters_for_gsl_optimizer * p_my_parameters = (user_parameters_for_gsl_optimizer *) _p_user_parameters;
    *f = p_my_parameters->last_value_f = my_function(_p_vector, _p_user_parameters);
    my_function_gradient(_p_vector, _p_user_parameters, _p_gradient_vector);
}

Automated_control_system * Regulator_tuner_interface::to_get_model()
{
    return acs_model;
}
PID_regulator * Regulator_tuner_interface::to_get_regulator()
{
    return regulator;
}
void Regulator_tuner_interface::to_set_model_and_regulator(Automated_control_system * _m, PID_regulator * _c)
{
    acs_model = _m;
    regulator = _c;
}
void Regulator_tuner_interface::to_set_model_and_regulator(std::shared_ptr<Automated_control_system> _m, std::shared_ptr<PID_regulator> _c)
{
    to_set_model_and_regulator(_m.get(), _c.get());
}
std::array<double, 3> Regulator_tuner_interface::to_get_solution() const
{
    return answer;
}
void Regulator_tuner_interface::to_reset_to_null()
{
    parameters_for_mine.p_tuner = nullptr;
    parameters_for_mine.p_acs_model = nullptr;
    parameters_for_mine.p_regulator = nullptr;
    parameters_for_mine.dt = 0;
    parameters_for_mine.length = 0;
    parameters_for_mine.t_registrate = 0;
    parameters_for_mine.times = 0;
    parameters_for_mine.min = 0;
    parameters_for_mine.max = 0;
    parameters_for_mine.last_value_f = 0;
    parameters_for_mine.h = 0;
}
void Regulator_tuner_interface::to_initialize()
{
    parameters_for_mine.p_tuner = this;
    if(acs_model != 0) parameters_for_mine.p_acs_model = acs_model;
    parameters_for_mine.p_regulator = regulator;
    if (acs_model != nullptr) parameters_for_mine.dt = acs_model->to_check_dt();
    parameters_for_mine.last_value_f = 0;
}


Regulator_tuner_with_GSL_interface::Regulator_tuner_with_GSL_interface(Automated_control_system * _m, PID_regulator * _r) : parameters(nullptr)
{
    to_set_model_and_regulator(_m, _r);
    to_reset_to_null();
    to_initialize();
}
Regulator_tuner_with_GSL_interface::~Regulator_tuner_with_GSL_interface()
{
    {
        if (x != nullptr) gsl_vector_free(x);
        if (my_minimizer_structure != nullptr) delete my_minimizer_structure;
    }
}
bool Regulator_tuner_with_GSL_interface::is_ready()
{
    if (parameters != nullptr) if (Regulator_tuner_interface::is_ready()) if (parameters->p_tuner != nullptr && parameters->p_acs_model != nullptr && parameters->p_regulator != nullptr && my_minimizer_structure != nullptr)
        return true;
    return false;
}

void Regulator_tuner_with_GSL_interface::to_reset_to_null()
{
    {
        if (x != nullptr) gsl_vector_free(x);
        if (my_minimizer_structure != nullptr) delete my_minimizer_structure;
    }
    if (parameters != nullptr)
    {
        if (parameters->p_tuner != nullptr) parameters->p_tuner = nullptr;
        if (parameters->p_acs_model != nullptr) parameters->p_acs_model = nullptr;
        if (parameters->p_regulator != nullptr) parameters->p_regulator = nullptr;
    }

    x = nullptr;
    my_minimizer_structure = nullptr;
    parameters = nullptr;
    iteration = 0;
    status = 0;
    s = nullptr;
}
void Regulator_tuner_with_GSL_interface::to_initialize()
{
    x = gsl_vector_alloc(3);
    if(regulator != nullptr)
    {
        gsl_vector_set(x, 0, regulator->to_check_parameters()[PID_regulator::K_P]);
        gsl_vector_set(x, 1, regulator->to_check_parameters()[PID_regulator::K_I]);
        gsl_vector_set(x, 2, regulator->to_check_parameters()[PID_regulator::K_D]);
    }

    my_minimizer_structure = new gsl_multimin_function_fdf;


    to_set_configurations(parameters); // is_ready = true if to_set_configurations preliminarily is called

    iteration = 0;
    status = 0;

}
void Regulator_tuner_with_GSL_interface::to_set_configurations(user_parameters_for_gsl_optimizer* _parameters)
{
    if (parameters != _parameters) parameters = _parameters;
    if (parameters != nullptr)
    {
        parameters->p_tuner = this;
        if (acs_model != nullptr) parameters->p_acs_model = acs_model;
        if (regulator != nullptr) parameters->p_regulator = regulator;
        if (acs_model != nullptr) parameters->dt = acs_model->to_check_dt();
        parameters->length = parameters_for_mine.length;
        parameters->t_registrate = parameters_for_mine.t_registrate;
        parameters->times = parameters_for_mine.times;
        parameters->min = parameters_for_mine.min;
        parameters->max = parameters_for_mine.max;
        parameters->last_value_f = 0;
        parameters->h = parameters_for_mine.h;
    }
    if (my_minimizer_structure != nullptr)
    {
        my_minimizer_structure->n = 3;
        my_minimizer_structure->f = my_function;
        my_minimizer_structure->df = my_function_gradient;
        my_minimizer_structure->fdf = my_functions_f_and_gradient;
        my_minimizer_structure->params = parameters;
    }
}

Regulator_tuner_gradient_method::Regulator_tuner_gradient_method(Automated_control_system * _m, PID_regulator * _r)
    : Regulator_tuner_with_GSL_interface(_m, _r), T(gsl_multimin_fdfminimizer_conjugate_fr)
{
    s = gsl_multimin_fdfminimizer_alloc(T, 3);
}
Regulator_tuner_gradient_method::~Regulator_tuner_gradient_method()
{
    if (s != nullptr) gsl_multimin_fdfminimizer_free(s);
}
void Regulator_tuner_gradient_method::to_reset_to_null()
{
    if (s != nullptr) gsl_multimin_fdfminimizer_free(s);
}
void Regulator_tuner_gradient_method::to_initialize()
{
    Regulator_tuner_with_GSL_interface::to_initialize();

    if (s != nullptr) s = gsl_multimin_fdfminimizer_alloc(T, 3);
}
#include <iostream>
void Regulator_tuner_gradient_method::to_tune()
{
    if (is_ready()) for(; iteration < 10; ++iteration)
    {
        status = gsl_multimin_fdfminimizer_iterate(s);
        std::cout << "iteration pre-break\n" << status << "\n";
        if (status) break;
        std::cout << "iteration\n";
        status = gsl_multimin_test_gradient(s->gradient, 1e-3);
    }
    else
        std::cout << "NOT iteration\n";
    std::cout << "WTF?\n";

    std::array<double, 3> arr {gsl_vector_get(x,0), gsl_vector_get(x,1), gsl_vector_get(x,2)};
    answer = arr;

    std::array<double, 3> k = arr;
    regulator->to_set_koefficients(k[0],k[1],k[2]);
}

bool Regulator_tuner_interface::is_ready()
{
    if ( parameters_for_mine.p_regulator != nullptr && parameters_for_mine.p_tuner != nullptr && parameters_for_mine.p_acs_model && acs_model != nullptr && regulator != nullptr)
        return true;
    return false;
}

bool Regulator_tuner_on_my_on_interface::is_ready()
{
    return Regulator_tuner_interface::is_ready();
}


Regulator_tuner_my_generic_algorithm::Regulator_tuner_my_generic_algorithm(Automated_control_system * _m, PID_regulator * _r)
{
    to_set_model_and_regulator(_m,_r);
}

void Regulator_tuner_my_generic_algorithm::to_tune()
{
    generative_algorithm<3> solver;
    solver.to_set_fitnes_function(fi);
    solver.to_set_min_init(0);
    solver.to_set_max_init(1);
    solver.to_set_mutation_step(0.1);
    solver.to_set_mutation_propability(0.1);
    solver.to_set_agents(1000);
    solver.to_set_new_agents(2000);
    solver.to_set_iterations(iteration);
    solver.to_set_fitness_function_parameters(&parameters_for_mine);
    auto m = solver.to_solve();

    std::array<double, 3> arr = m.second;
    answer = arr;

    std::array<double, 3> k = arr;
    regulator->to_set_koefficients(k[0],k[1],k[2]);
}

Regulator_tuner::Regulator_tuner()
{

}
Regulator_tuner::~Regulator_tuner()
{

}

void Regulator_tuner::to_tune()
{}
