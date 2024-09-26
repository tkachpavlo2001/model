#ifndef REGULATOR_TUNER_H
#define REGULATOR_TUNER_H

#include"automated_control_system.h"

//#include<gsl/gsl_multimin.h>

//#include <memory>
//#include <array>

//#include "generative_algorithm.h"
struct parameters_for_fitness_function
{
    double dt = 0;
    double length = 0;
    double t_registrate = 0;
};

struct parameters_for_varied_fitness_function
{
    double times = 0;
    double min = 0;
    double max = 0;
};

struct parameters_for_gradient
{
    double last_value_f = 0;
    double dx = 0;
    double learn_step = 0;
};


struct parameters_p_objects
{
    Automated_control_system * p_acs_model = nullptr;
    PID_regulator * p_regulator = nullptr;
};

struct parameters_for_optimizer
{
    parameters_p_objects parameters_p_objects_parameters_obj;
    parameters_for_fitness_function parameters_for_fitness_function_obj;
    parameters_for_varied_fitness_function parameters_for_varied_fitness_function_obj;
    parameters_for_gradient parameters_for_gradient_obj;
};

long double fitness_function_varied_reference_signal(double*,void*);

long double gradient_by_step(double*,void*);

class Regulator_tuner_interface
{
    ;
};

class Regulator_tuner_automated_manual_algorithm_interface : virtual public Regulator_tuner_interface
{};

class Regulator_tuner_my_optimizer_interface : virtual public Regulator_tuner_interface
{};

class Regulator_tuner_side_optimizer_interface : virtual public Regulator_tuner_interface
{};

class Regulator_tuner_my_generative_algorithm : Regulator_tuner_my_optimizer_interface
{};

class Regulator_tuner
{};
#endif // REGULATOR_TUNER_H
