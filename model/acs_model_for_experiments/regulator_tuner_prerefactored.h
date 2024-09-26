#ifndef REGULATOR_TUNER_H
#define REGULATOR_TUNER_H


#include"automated_control_system.h"

#include<gsl/gsl_multimin.h>

#include <memory>
#include <array>

#include "generative_algorithm.h"

class Regulator_tuner_interface;
long double fitness_function_varied_reference_signal
(Regulator_tuner_interface* _tuner, double _dt, double _length, double _t_registrate, double _times, double _min, double _max);
struct user_parameters_for_gsl_optimizer
{
    Regulator_tuner_interface * p_tuner = nullptr;
    Automated_control_system * p_acs_model = nullptr;
    PID_regulator * p_regulator = nullptr;
    double dt = 0;
    double length = 0;
    double t_registrate = 0;
    double times = 0;
    double min = 0;
    double max = 0;
    double last_value_f = 0;
    double h = 0;
};
long double fitness_function_varied_reference_signal
(user_parameters_for_gsl_optimizer*);
double my_function_for_mine(double*,void*);
double my_function(const gsl_vector *, void *);
void my_function_gradient(const gsl_vector *, void *, gsl_vector *);
void my_functions_f_and_gradient(const gsl_vector*, void*, double*, gsl_vector*);

class Regulator_tuner_interface
{
protected:
    Automated_control_system * acs_model;
    PID_regulator * regulator;
    virtual bool is_ready() = 0;
    std::array<double, 3> answer;
    user_parameters_for_gsl_optimizer parameters_for_mine;
    unsigned int iteration = 0;
public:
    Automated_control_system * to_get_model();
    PID_regulator * to_get_regulator();
    void to_set_model_and_regulator(std::shared_ptr<Automated_control_system>, std::shared_ptr<PID_regulator>);
    void to_set_model_and_regulator(Automated_control_system *, PID_regulator *);
    virtual std::array<double, 3> to_get_solution() const;
    virtual void to_tune() = 0;
    virtual void to_reset_to_null();
    virtual void to_initialize();

    void to_set_length(double _num) { parameters_for_mine.length = _num; }
    void to_set_t_registrate(double _num) { parameters_for_mine.t_registrate = _num;}
    void to_set_times(double _num) { parameters_for_mine.times = _num;}
    void to_set_min(double _num) { parameters_for_mine.min = _num;}
    void to_set_max(double _num) { parameters_for_mine.max = _num;}
    void to_set_h(double _num) { parameters_for_mine.h = _num;}
    void to_set_iterations(double _num) { iteration = _num;}
};

class Regulator_tuner_with_side_library_interface : virtual public Regulator_tuner_interface {};
class Regulator_tuner_on_my_on_interface : virtual public Regulator_tuner_interface
{
protected:
    bool is_ready();
};

class Regulator_tuner_with_GSL_interface : public Regulator_tuner_with_side_library_interface
{
private:
protected:
    virtual bool is_ready() override;
    gsl_vector * x = nullptr;
    gsl_multimin_function_fdf * my_minimizer_structure = nullptr;
    user_parameters_for_gsl_optimizer * parameters = nullptr;
    unsigned int iteration = 0;
    int status = 0;
    gsl_multimin_fdfminimizer * s = nullptr;
public:
    Regulator_tuner_with_GSL_interface(Automated_control_system * = nullptr, PID_regulator * = nullptr);
    virtual ~Regulator_tuner_with_GSL_interface() = 0;
    virtual void to_reset_to_null() override;
    virtual void to_initialize() override;
    void to_set_configurations(user_parameters_for_gsl_optimizer*);
};

class Regulator_tuner_gradient_method : virtual public Regulator_tuner_with_GSL_interface
{
private:
    const gsl_multimin_fdfminimizer_type * T;
public:
    Regulator_tuner_gradient_method(Automated_control_system * = nullptr, PID_regulator * = nullptr);
    virtual ~Regulator_tuner_gradient_method();
    virtual void to_initialize() override;
    virtual void to_reset_to_null() override;
    void to_tune() override;
};

class Regulator_tuner_my_generic_algorithm : virtual public Regulator_tuner_on_my_on_interface
{
    double (*fi)(double*, void*) = my_function_for_mine;
public:
    Regulator_tuner_my_generic_algorithm(Automated_control_system * = nullptr, PID_regulator * = nullptr);
    virtual void to_tune();
};
class Regulator_tuner_my_gradient : virtual public Regulator_tuner_on_my_on_interface
{
    ;
};

class Regulator_tuner : public Regulator_tuner_with_GSL_interface
{
private:
public:
    Regulator_tuner();
    virtual ~Regulator_tuner();
    void to_tune() override;
};

#endif // REGULATOR_TUNER_H
