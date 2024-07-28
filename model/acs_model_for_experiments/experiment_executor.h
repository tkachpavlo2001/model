#ifndef EXPERIMENT_EXECUTOR_H
#define EXPERIMENT_EXECUTOR_H

#include"automated_control_system.h"
#include"string"

class Experiment_executor
{
    Automated_control_system * acs_model;

    std::string results_title;
    double dt;
    double t_begin;
    double t_length;
    double time_to_show;
    double interval;
    double amount_of_show;
    void reset_interval();
public:
    Experiment_executor(Automated_control_system * = nullptr);
    void to_get_model_to_run(Automated_control_system *);
    void to_set_result_title(const char *);
    void to_run() const;
    void to_set_dt(double);
    void to_set_t_begin(double);
    void to_set_t_length(double);
    void to_set_time_to_registrate(double);
    void to_set_amount_of_registrations(double);
};

#endif // EXPERIMENT_EXECUTOR_H
