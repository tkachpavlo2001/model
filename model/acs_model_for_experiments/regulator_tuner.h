#ifndef REGULATOR_TUNER_H
#define REGULATOR_TUNER_H


#include"automated_control_system.h"

#include<gsl/gsl_multimin.h>

#include <memory>

class Regulator_tuner_iterface;
long double fitness_function_varied_reference_signal
(Regulator_tuner_iterface* _tuner, double _dt, double _length, double _t_registrate, double _times, double _min, double _max);
struct user_parameters_for_gsl_optimizer
{
    Regulator_tuner_iterface * p_tuner;
    Automated_control_system * p_acs_model;
    PID_regulator * p_regulator;
    double dt;
    double length;
    double t_registrate;
    double times;
    double min;
    double max;
    double last_value_f;
    double h;
};
long double fitness_function_varied_reference_signal
(user_parameters_for_gsl_optimizer*);
double my_function(const gsl_vector *, void *);
void my_function_gradient(const gsl_vector *, void *, gsl_vector *);
void my_functions_f_and_gradient(const gsl_vector*, void*, double*, gsl_vector*);

class Regulator_tuner_iterface
{
private:
    Automated_control_system * acs_model;
    PID_regulator * regulator;
public:
    Regulator_tuner_iterface(Automated_control_system * = nullptr, PID_regulator * = nullptr);
    virtual ~Regulator_tuner_iterface() = 0;
    void to_set_model_and_regulator(std::shared_ptr<Automated_control_system>, std::shared_ptr<PID_regulator>);
    void to_set_model_and_regulator(Automated_control_system *, PID_regulator *);
    Automated_control_system * to_get_model();
    PID_regulator * to_get_regulator();
};

class Regulator_tuner_gradient_method : virtual public Regulator_tuner_iterface
{
private:
    const gsl_multimin_fdfminimizer_type * T;
    gsl_multimin_fdfminimizer * s = nullptr;
    gsl_vector * x;
    gsl_multimin_function_fdf my_function;
public:
    Regulator_tuner_gradient_method(Automated_control_system * = nullptr, PID_regulator * = nullptr);
    virtual ~Regulator_tuner_gradient_method();
};

class Regulator_tuner : public Regulator_tuner_iterface
{
private:
public:
    Regulator_tuner();
    virtual ~Regulator_tuner();
};

#endif // REGULATOR_TUNER_H
