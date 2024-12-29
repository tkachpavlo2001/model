/*
 * This file is part of ACS_MODEL.
 *
 * ACS_MODEL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACS_MODEL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACS_MODEL. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef EXPERIMENT_EXECUTOR_H
#define EXPERIMENT_EXECUTOR_H

#include"automated_control_system.h"
#include"registrator.h"

#include<string>
#include<vector>
#include<memory>

class Experiment_executor_interface
{
private:
    Automated_control_system * acs_model;
    double dt;
    double t_length;
    double time_to_show;
    double interval;
    double amount_of_show;
protected:
    Registrator * p_registrator = nullptr;
    void reset_interval();
    virtual void to_run(Registrator *) const;
    double to_get_t_length() const;
    double to_get_interval() const;
    Automated_control_system * to_get_model() const;
public:
    Experiment_executor_interface(Automated_control_system * = nullptr);
    virtual ~Experiment_executor_interface() = 0;
    virtual void to_run() = 0;
    void to_get_model_to_run(Automated_control_system *);
    Automated_control_system * to_get_model();
    void to_set_dt(double);
    void to_set_t_length(double);
    void to_set_time_to_registrate(double);
    void to_set_amount_of_registrations(double);
    void to_set_registrator(Registrator * p) { p_registrator = p; }
    double to_check_time_to_show() { return time_to_show; }
};

class Experiment_executor : virtual public Experiment_executor_interface // for txt-file report
{
protected:
    std::string results_title;
public:
    Experiment_executor(Automated_control_system * = nullptr);
    void to_set_result_title(const char *);
    virtual void to_run() override;
};

class Experiment_executor_short_report : public Experiment_executor // for short txt-file report
{
public:
    Experiment_executor_short_report(Automated_control_system * = nullptr);
    virtual void to_run() override;
};

class Experiment_executor_for_fitness_function : virtual public Experiment_executor_interface
{
protected:
    std::vector<double> * records = nullptr;
public:
    Experiment_executor_for_fitness_function(Automated_control_system * = nullptr);
    void to_set_vector(std::vector<double>*);
    void to_set_vector(std::vector<double>&);
    void to_set_vector(std::shared_ptr<std::vector<double>>);
    virtual void to_run() override;
};

class Experiment_executor_velocity_record : public Experiment_executor_for_fitness_function
{
public:
    void to_run() override;
};

class Experiment_executor_for_fitness_function_with_varied_reference_signal : public Experiment_executor_for_fitness_function
{
    double reference_signal_max;
    double reference_signal_min;
public:
    Experiment_executor_for_fitness_function_with_varied_reference_signal(Automated_control_system * = nullptr);

    void to_set_varied_diapasone_min_max(double,double);

    virtual void to_run() override;
};


#endif // EXPERIMENT_EXECUTOR_H
