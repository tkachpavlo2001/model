#include "experiment_executor.h"
#include"registrator.h"

void Experiment_executor::reset_interval()
{
    if (times != 0)
    {
        ;
    }
    if (amount != 0)
    {
        ;
    }
}
Experiment_executor::Experiment_executor(Automated_control_system * arg) : acs_model(arg), dt(0), t_begin(0), t_length(1), time_to_show(1)
{
    if (acs_model != nullptr) dt = acs_model->to_check_dt();
    if (acs_model != nullptr) t_begin = acs_model->to_check_t();
}

void Experiment_executor::to_get_model_to_run(Automated_control_system * arg)
{
    acs_model = arg;
    if (acs_model != nullptr) dt = acs_model->to_check_dt();
    if (acs_model != nullptr) t_begin = acs_model->to_check_t();
}

void Experiment_executor::to_run() const
{
    double current_time = acs_model->to_check_t();
    double previous_time = current_time;
    double last_show_time = current_time;

    Registrator_to_txt_file fout;

    while(current_time <= t_begin + t_length)
    {
        acs_model->to_calculate();
        previous_time = current_time;
        current_time = acs_model->to_check_t();
        if (current_time - last_show_time >= time_to_show)
        {
            fout << *acs_model;
            last_show_time = current_time;
        }
    }
    // t in acs is not fixed by registrator. To be fixed!
}

void Experiment_executor::to_set_dt(double _dt)
{
    dt = _dt;
    if (acs_model != nullptr) acs_model->to_set_dt(dt);
    reset_interval();
}

void Experiment_executor::to_set_t_begin(double _t_begin)
{
    t_begin = _t_begin;
    reset_interval();
}

void Experiment_executor::to_set_t_length(double _t_length)
{
    t_length = _t_length;
    reset_interval();
}

void Experiment_executor::to_set_time_to_registrate(double _times)
{
    times = _times;
    amount = 0;
    reset_interval();
}

void Experiment_executor::to_set_amount_of_registrations(double _amount)
{
    amount = _amount;
    times = 0;
    reset_interval();
}
