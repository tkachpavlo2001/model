#include "experiment_executor.h"
#include"registrator.h"

void Experiment_executor::reset_interval()
{
    if (interval != 0)
    {
        time_to_show = interval;
    }
    if (amount_of_show != 0)
    {
        time_to_show = t_length / amount_of_show;
    }
}
Experiment_executor::Experiment_executor(Automated_control_system * arg)
    : acs_model(arg), dt(0), t_begin(0), t_length(0), time_to_show(0), interval(0), amount_of_show(0)
{
    if (acs_model != nullptr) dt = acs_model->to_check_dt();
    if (acs_model != nullptr) t_begin = acs_model->to_check_t();
}

void Experiment_executor::to_get_model_to_run(Automated_control_system * arg)
{
    if (arg != acs_model)
    {
        acs_model = arg;
        if (acs_model != nullptr) dt = acs_model->to_check_dt();
        if (acs_model != nullptr) t_begin = acs_model->to_check_t();
    }
}

void Experiment_executor::to_set_result_title(const char * _title)
{
    results_title = _title;
}

void Experiment_executor::to_run() const
{
    double current_time = 0;
    if (acs_model != nullptr) current_time = acs_model->to_check_t();
    double last_show_time = current_time;

    Registrator * own_registrator = new Registrator_to_txt_file;
    Registrator & fout = *own_registrator;
    fout.to_set_name_of_file(results_title);

    fout << *acs_model;
    if (acs_model != nullptr) while(current_time <= t_begin + t_length)
    {
        acs_model->to_calculate();
        current_time = acs_model->to_check_t();
        if (current_time - last_show_time >= time_to_show)
        {
            fout << *acs_model;
            last_show_time = current_time;
        }
    }
    fout << *acs_model;

    delete own_registrator;
}


void Experiment_executor::to_run(Registrator * own_registrator) const
{
    double current_time = 0;
    if (acs_model != nullptr) current_time = acs_model->to_check_t();
    double last_show_time = current_time;

    Registrator & fout = *own_registrator;
    fout.to_set_name_of_file(results_title);

    fout << *acs_model;
    if (acs_model != nullptr) while(current_time <= t_begin + t_length)
    {
        acs_model->to_calculate();
        current_time = acs_model->to_check_t();
        if (current_time - last_show_time >= time_to_show)
        {
            fout << *acs_model;
            last_show_time = current_time;
        }
    }
    fout << *acs_model;

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

void Experiment_executor::to_set_time_to_registrate(double _interval)
{
    interval = _interval;
    amount_of_show = 0;
    reset_interval();
}

void Experiment_executor::to_set_amount_of_registrations(double _amount_of_show)
{
    amount_of_show = _amount_of_show;
    interval = 0;
    reset_interval();
}

void Experiment_executor_short_report::to_run() const
{
    Registrator * own_registrator = new Registrator_to_txt_file_short;
    Experiment_executor::to_run(own_registrator);
    delete own_registrator;
}
