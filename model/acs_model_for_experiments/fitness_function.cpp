#include "regulator_tuner.h"
#include "experiment_executor.h"

#include "tkachpavlo2001lib/myExeption.hpp"

#include <numeric>

class exeption_for_fitness_function_varied_reference_signal : public myException
{
public:
    exeption_for_fitness_function_varied_reference_signal() : myException() {to_sent_message();}
    void to_sent_message() const override { std::cerr << "::fitness_function_varied_reference_signal"; }
};

long double fitness_function_varied_reference_signal(double*ans,void*param)
{
    int status = 0;
    parameters_for_optimizer * p_parameters_for_optimizer;
    std::shared_ptr<Experiment_executor_for_fitness_function_with_varied_reference_signal> p_experiment;
    try
    {
        if (param != nullptr)  p_parameters_for_optimizer = static_cast<parameters_for_optimizer *>(param);
        else
        {
            status = 1;
            throw exeption_for_fitness_function_varied_reference_signal();
        }
        if (p_parameters_for_optimizer == nullptr)
        {
            status = 2;
            throw exeption_for_fitness_function_varied_reference_signal();
        }
        else std::make_shared<Experiment_executor_for_fitness_function_with_varied_reference_signal>
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

        p_experiment->to_get_model_to_run(p_experiment->to_get_model());

        for (int i = 0; i < p_parameters_for_optimizer->parameters_for_varied_fitness_function_obj.times; ++i) p_experiment->to_run();

        return std::accumulate(records.begin(), records.end(), 0, [&](double acc, double num)->double {return acc + num * num;} );

    }

    catch (exeption_for_fitness_function_varied_reference_signal&)
    {
        std::cerr << ": status =" << status << std::endl << std::endl << std::endl;
        std::abort();
    }

}

long double gradient_by_step(double*ans,void*param)
{
    ;
}
