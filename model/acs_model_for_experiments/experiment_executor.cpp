#include "experiment_executor.h"
#include"registrator.h"

void Experiment_executor_interface::reset_interval()
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
void Experiment_executor_interface::to_run(Registrator * own_registrator) const
{
    Registrator & fout = *own_registrator;

    double current_time;
    if (acs_model != nullptr) current_time = acs_model->to_check_t();
    double start_time = current_time;
    double last_show_time = current_time;

    unsigned int show_num = 0;
    fout << *acs_model;
    if (acs_model != nullptr) while(current_time <= start_time + t_length)
    {
        if (current_time - last_show_time >= time_to_show)
        {
            fout << *acs_model;
            ++show_num;
            last_show_time = start_time + show_num * time_to_show;
        }
        acs_model->to_calculate();
        current_time = acs_model->to_check_t();
    }
    fout << *acs_model;

}
double Experiment_executor_interface::to_get_t_length() const
{
    return t_length;
}
double Experiment_executor_interface::to_get_interval() const
{
    return interval;
}
Automated_control_system * Experiment_executor_interface::to_get_model() const
{
    return acs_model;
}
Automated_control_system * Experiment_executor_interface::to_get_model()
{
    return acs_model;
}
Experiment_executor_interface::Experiment_executor_interface(Automated_control_system * arg)
    : acs_model(arg), dt(0), t_length(0), time_to_show(0), interval(0), amount_of_show(0)
{
    if (acs_model != nullptr) dt = acs_model->to_check_dt();
}
Experiment_executor_interface::~Experiment_executor_interface()
{}
void Experiment_executor_interface::to_get_model_to_run(Automated_control_system * arg)
{
    if (arg != acs_model)
    {
        acs_model = arg;
        if (acs_model != nullptr) dt = acs_model->to_check_dt();
    }
}
void Experiment_executor_interface::to_set_dt(double _dt)
{
    dt = _dt;
    if (acs_model != nullptr) acs_model->to_set_dt(dt);
    reset_interval();
}
void Experiment_executor_interface::to_set_t_length(double _t_length)
{
    t_length = _t_length;
    reset_interval();
}
void Experiment_executor_interface::to_set_time_to_registrate(double _interval)
{
    interval = _interval;
    amount_of_show = 0;
    reset_interval();
}
void Experiment_executor_interface::to_set_amount_of_registrations(double _amount_of_show)
{
    amount_of_show = _amount_of_show;
    interval = 0;
    reset_interval();
}

Experiment_executor::Experiment_executor(Automated_control_system * arg) : Experiment_executor_interface(arg)
{
}
void Experiment_executor::to_set_result_title(const char * _title)
{
    results_title = _title;
}
void Experiment_executor::to_run()
{

    Registrator * own_registrator;
    if (p_registrator == nullptr) own_registrator = new Registrator_to_txt_file;
    else own_registrator = p_registrator;
    Registrator & fout = *own_registrator;
    //fout.to_set_name_of_file(results_title);

    Experiment_executor_interface::to_run(own_registrator);

    if (p_registrator == nullptr) delete own_registrator;
}

Experiment_executor_short_report::Experiment_executor_short_report(Automated_control_system * arg) : Experiment_executor(arg), Experiment_executor_interface(arg)
{}
void Experiment_executor_short_report::to_run()
{
    Registrator * own_registrator = new Registrator_to_txt_file_short;
    Registrator & fout = *own_registrator;
    fout.to_set_name_of_file(results_title);
    Experiment_executor_interface::to_run(own_registrator);
    delete own_registrator;
}

Experiment_executor_for_fitness_function::Experiment_executor_for_fitness_function(Automated_control_system * arg) : Experiment_executor_interface(arg)
{}
void Experiment_executor_for_fitness_function::to_set_vector(std::vector<double>* _vector)
{
    records = _vector;
}
void Experiment_executor_for_fitness_function::to_set_vector(std::vector<double>& _vector)
{
    to_set_vector(&_vector);
}
void Experiment_executor_for_fitness_function::to_set_vector(std::shared_ptr<std::vector<double>> _vector)
{
    to_set_vector(_vector.get());
}
void Experiment_executor_for_fitness_function::to_run()
{
    Registrator_to_std_vector_difference * own_registrator = new Registrator_to_std_vector_difference;
    own_registrator->to_set_vector(records);
    Experiment_executor_interface::to_run(own_registrator);
    delete own_registrator;
}

void Experiment_executor_velocity_record::to_run()
{
    Registrator_to_std_vector * own_registrator = new Registrator_to_std_vector;
    own_registrator->to_set_vector(records);
    Experiment_executor_interface::to_run(own_registrator);
    delete own_registrator;
}

Experiment_executor_for_fitness_function_with_varied_reference_signal::Experiment_executor_for_fitness_function_with_varied_reference_signal(Automated_control_system * arg)
    : Experiment_executor_interface(arg), Experiment_executor_for_fitness_function(arg)
{}

void Experiment_executor_for_fitness_function_with_varied_reference_signal::to_set_varied_diapasone_min_max(double _min, double _max)
{
    reference_signal_max = _max;
    reference_signal_min = _min;
}

void Experiment_executor_for_fitness_function_with_varied_reference_signal::to_run()
{
    Registrator_to_std_vector_difference * own_registrator = new Registrator_to_std_vector_difference;
    own_registrator->to_set_vector(records);
    double t_length_original = Experiment_executor_interface::to_get_t_length();
    Experiment_executor_interface::to_set_t_length(t_length_original/2);
    Automated_control_system_element_interface * p_acs_model_element = Experiment_executor_interface::to_get_model()->to_get_definder();
    Reference_signal_definder_static * p_definder = (Reference_signal_definder_static*) p_acs_model_element;
    p_definder->to_set_signal(reference_signal_max);
    Experiment_executor_interface::to_run(own_registrator);
    p_definder->to_set_signal(reference_signal_min);
    Experiment_executor_interface::to_run(own_registrator);
    Experiment_executor_interface::to_set_t_length(t_length_original);
    delete own_registrator;
}
