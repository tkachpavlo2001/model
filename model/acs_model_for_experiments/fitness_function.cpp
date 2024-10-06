#include "regulator_tuner.h"
#include "experiment_executor.h"

#include "tkachpavlo2001lib/myExeption.hpp"

#include <numeric>

class exception_for_fitness_function_varied_reference_signal : public myException
{
public:
    exception_for_fitness_function_varied_reference_signal() : myException() {std::cerr << "::fitness_function_varied_reference_signal"; }
};

class exception_for_gradient_by_step : public myException
{
public:
    exception_for_gradient_by_step() : myException() { std::cerr << "::gradient_by_step"; }
};

double fitness_function_varied_reference_signal(double*ans,void*param)
{
    int status = -1;
    parameters_for_optimizer * p_parameters_for_optimizer;
    std::shared_ptr<Experiment_executor_for_fitness_function_with_varied_reference_signal> p_experiment;
    try
    {
        if (param != nullptr)  p_parameters_for_optimizer = static_cast<parameters_for_optimizer *>(param);
        else
        {
            status = 1;
            throw exception_for_fitness_function_varied_reference_signal();
        }
        if (p_parameters_for_optimizer == nullptr)
        {
            status = 2;
            throw exception_for_fitness_function_varied_reference_signal();
        }
        else p_experiment = std::make_shared<Experiment_executor_for_fitness_function_with_varied_reference_signal>
                (
                    p_parameters_for_optimizer->parameters_p_objects_parameters_obj.p_acs_model
                );

        p_experiment->to_set_t_length(p_parameters_for_optimizer->parameters_for_fitness_function_obj.length);
        p_experiment->to_set_dt(p_parameters_for_optimizer->parameters_for_fitness_function_obj.dt);
        p_experiment->to_set_varied_diapasone_min_max
                (
                    p_parameters_for_optimizer->parameters_for_varied_fitness_function_obj.min,
                    p_parameters_for_optimizer->parameters_for_varied_fitness_function_obj.max
                );
        p_experiment->to_set_time_to_registrate(p_parameters_for_optimizer->parameters_for_fitness_function_obj.t_registrate);

        std::vector<double> records;
        p_experiment->to_set_vector(records);

        p_experiment->to_get_model_to_run(p_parameters_for_optimizer->parameters_p_objects_parameters_obj.p_acs_model);

        for (int i = 0; i < p_parameters_for_optimizer->parameters_for_varied_fitness_function_obj.times; ++i) p_experiment->to_run();

        status = 0;
        return std::accumulate(records.begin(), records.end(), 0, [&](double acc, double num)->double {return acc + num * num;} );
    }
    catch (exception_for_fitness_function_varied_reference_signal&)
    {
        std::cerr << ": status = " << status << std::endl << std::endl << std::endl;
        std::abort();
    }

}

std::array<long double, 3> gradient_by_step(double*ans,void*param)
{
    int status = -1;
    parameters_for_optimizer * p_parameters_for_optimizer;
    std::array<long double, 3> gradient {0};
    try
    {
        if (param == nullptr)
        {
            status = 1;
            throw exception_for_gradient_by_step();
        }
        p_parameters_for_optimizer = static_cast<parameters_for_optimizer*>(param);
        if (p_parameters_for_optimizer == nullptr)
        {
            status = 2;
            throw exception_for_gradient_by_step();
        }

        PID_regulator * p_regulator = p_parameters_for_optimizer->parameters_p_objects_parameters_obj.p_regulator;
        double temp;

        temp = p_regulator->to_get_parameters()[PID_regulator::K_P];
        p_regulator->to_get_parameters()[PID_regulator::K_P] += p_parameters_for_optimizer->parameters_for_gradient_obj.dx;
        gradient[0] = fitness_function_varied_reference_signal(ans, param);
        p_regulator->to_get_parameters()[PID_regulator::K_P] = temp;

        temp = p_regulator->to_get_parameters()[PID_regulator::K_I];
        p_regulator->to_get_parameters()[PID_regulator::K_I] += p_parameters_for_optimizer->parameters_for_gradient_obj.dx;
        gradient[1] = fitness_function_varied_reference_signal(ans, param);
        p_regulator->to_get_parameters()[PID_regulator::K_I] = temp;

        temp = p_regulator->to_get_parameters()[PID_regulator::K_D];
        p_regulator->to_get_parameters()[PID_regulator::K_D] += p_parameters_for_optimizer->parameters_for_gradient_obj.dx;
        gradient[2] = fitness_function_varied_reference_signal(ans, param);
        p_regulator->to_get_parameters()[PID_regulator::K_D] = temp;

        status = 0;
        return gradient;
    }
    catch (exception_for_gradient_by_step&)
    {
        std::cerr << ": status = " << status << std::endl << std::endl << std::endl;
        std::abort();
    }
}

