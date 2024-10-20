#ifndef REGULATOR_TUNER_H
#define REGULATOR_TUNER_H

#include"automated_control_system.h"

//#include<gsl/gsl_multimin.h>

//#include <memory>
//#include <array>

#include "generative_algorithm.h"
#include "gradient_method.h"

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

inline void to_copy_parameters_p_objects(parameters_p_objects& _receiver, parameters_p_objects& _sender)
{
    _receiver.p_acs_model = _sender.p_acs_model;
    _receiver.p_regulator = _sender.p_regulator;
}

struct parameters_configurations_for_optimizer
{
    unsigned int iterations = 0;
    unsigned int agents = 0;
    unsigned int new_agents = 0;
    double mutation_step = 0;
    double mutation_propability = 0;
    double min_init = 0;
    double max_init = 0;
};

struct parameters_for_optimizer
{
    parameters_p_objects parameters_p_objects_parameters_obj;

    parameters_for_fitness_function parameters_for_fitness_function_obj;
    parameters_for_varied_fitness_function parameters_for_varied_fitness_function_obj;
    parameters_for_gradient parameters_for_gradient_obj;

    parameters_configurations_for_optimizer parameters_configurations_for_optimizer_obj;
};

inline void to_copy_parameters_for_optimizer(parameters_for_optimizer& _receiver, parameters_for_optimizer& _sender)
{
    to_copy_parameters_p_objects(_receiver.parameters_p_objects_parameters_obj, _sender.parameters_p_objects_parameters_obj);
    _receiver.parameters_for_fitness_function_obj = _sender.parameters_for_fitness_function_obj;
    _receiver.parameters_for_varied_fitness_function_obj = _sender.parameters_for_varied_fitness_function_obj;
    _receiver.parameters_for_gradient_obj = _sender.parameters_for_gradient_obj;
    _receiver.parameters_configurations_for_optimizer_obj = _sender.parameters_configurations_for_optimizer_obj;
}

double fitness_function_varied_reference_signal(double*,void*);

std::array<long double, 3> gradient_by_step(double*,void*);

class Regulator_tuner_interface
{
private:
    std::array<double, 3> answer { 0, 0, 0 };
protected:
    parameters_for_optimizer * p_parameters = nullptr;

    bool parameters_is_null() { return p_parameters == nullptr; }
    void to_set_answer(std::array<double, 3> _arg) { answer = _arg; }
public:
    Regulator_tuner_interface (parameters_for_optimizer&);
    Regulator_tuner_interface ();
    virtual ~Regulator_tuner_interface() = 0;

    void to_set_parameters(parameters_for_optimizer&);
    void to_reset_parameters();
    std::array<double, 3> & to_check_answer();

    virtual void to_tune() = 0;
};

class Regulator_tuner_automated_manual_algorithm_interface : virtual public Regulator_tuner_interface
{};

class Regulator_tuner_my_optimizer_interface : virtual public Regulator_tuner_interface
{
public:
    Regulator_tuner_my_optimizer_interface() {}
    virtual ~Regulator_tuner_my_optimizer_interface() = 0;
    virtual void to_tune() = 0;
};

class Regulator_tuner_side_optimizer_interface : virtual public Regulator_tuner_interface
{};

class Regulator_tuner_my_generative_algorithm : public Regulator_tuner_my_optimizer_interface
{
private:
    generative_algorithm<3> generative_algorithm_obj;
public:
    Regulator_tuner_my_generative_algorithm(parameters_for_optimizer& _parameters_to_set) : Regulator_tuner_interface(_parameters_to_set) {}
    Regulator_tuner_my_generative_algorithm() {}
    virtual ~Regulator_tuner_my_generative_algorithm() {}

    void to_tune() override;
};

class Regulator_tuner_my_gradient_algorithm : public Regulator_tuner_my_optimizer_interface
{
private:
    stochastic_gradient_method_step_based<3> stochastic_gradient_method_step_based_obj;
public:
    Regulator_tuner_my_gradient_algorithm(parameters_for_optimizer& _parameters_to_set) : Regulator_tuner_interface(_parameters_to_set) {}
    Regulator_tuner_my_gradient_algorithm() {}
    virtual ~Regulator_tuner_my_gradient_algorithm() {}

    void to_tune() override;
};

class Regulator_tuner
{};
#endif // REGULATOR_TUNER_H
