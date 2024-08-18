#ifndef REGULATOR_TUNER_H
#define REGULATOR_TUNER_H


#include"automated_control_system.h"

#include<gsl/gsl_multimin.h>

#include <memory>
#include <array>

class Regulator_tuner_interface;
long double fitness_function_varied_reference_signal
(Regulator_tuner_interface* _tuner, double _dt, double _length, double _t_registrate, double _times, double _min, double _max);
struct user_parameters_for_gsl_optimizer
{
    Regulator_tuner_interface * p_tuner;
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

class Regulator_tuner_interface
{
private:
    Automated_control_system * acs_model;
    PID_regulator * regulator;
protected:
    bool is_ready = false;
    bool command_to_delete = false;
    gsl_vector * x = nullptr;
    gsl_multimin_function_fdf * my_minimizer_structure;
    user_parameters_for_gsl_optimizer * parameters;
    unsigned int iteration = 0;
    int status = 0;
    gsl_multimin_fdfminimizer * s = nullptr;
public:
    Regulator_tuner_interface(Automated_control_system * = nullptr, PID_regulator * = nullptr);
    virtual ~Regulator_tuner_interface() = 0;
    void to_set_model_and_regulator(std::shared_ptr<Automated_control_system>, std::shared_ptr<PID_regulator>);
    void to_set_model_and_regulator(Automated_control_system *, PID_regulator *);
    Automated_control_system * to_get_model();
    PID_regulator * to_get_regulator();
    virtual void to_reset_to_null();
    virtual void to_initialize();
    void to_set_configurations(user_parameters_for_gsl_optimizer*);
    std::array<double, 3> to_get_solution() const;
};

class Regulator_tuner_gradient_method : virtual public Regulator_tuner_interface
{
private:
    const gsl_multimin_fdfminimizer_type * T;
public:
    Regulator_tuner_gradient_method(Automated_control_system * = nullptr, PID_regulator * = nullptr);
    virtual ~Regulator_tuner_gradient_method();
    virtual void to_initialize() override;
    virtual void to_reset_to_null() override;
    void to_tune();
};

class Regulator_tuner : public Regulator_tuner_interface
{
private:
public:
    Regulator_tuner();
    virtual ~Regulator_tuner();
};

#endif // REGULATOR_TUNER_H
