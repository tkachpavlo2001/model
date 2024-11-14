#include "regulator_tuner.h"

Regulator_tuner_interface::Regulator_tuner_interface(parameters_for_optimizer& _parameters_to_set)
{
    p_parameters = new parameters_for_optimizer;
    to_copy_parameters_for_optimizer(*p_parameters, _parameters_to_set);
}

Regulator_tuner_interface::Regulator_tuner_interface() : p_parameters(nullptr)
{
    ;
}

Regulator_tuner_interface::~Regulator_tuner_interface()
{
    if (!parameters_is_null()) delete p_parameters;
}

void Regulator_tuner_interface::to_set_parameters(parameters_for_optimizer& _parameters_to_set)
{
    if(parameters_is_null()) p_parameters = new parameters_for_optimizer;
    to_copy_parameters_for_optimizer(*p_parameters, _parameters_to_set);
}


void Regulator_tuner_interface::to_reset_parameters()
{
    if(!parameters_is_null()) delete p_parameters;
}


std::array<double, 3> & Regulator_tuner_interface::to_check_answer()
{
    return answer;
}

Regulator_tuner_my_optimizer_interface::~Regulator_tuner_my_optimizer_interface() {}

void Regulator_tuner_my_generative_algorithm::to_tune()
{
    generative_algorithm_obj.to_set_min_init(p_parameters->parameters_configurations_for_optimizer_obj.min_init);
    generative_algorithm_obj.to_set_max_init(p_parameters->parameters_configurations_for_optimizer_obj.max_init);
    generative_algorithm_obj.to_set_mutation_step(p_parameters->parameters_configurations_for_optimizer_obj.mutation_step);
    generative_algorithm_obj.to_set_mutation_propability(p_parameters->parameters_configurations_for_optimizer_obj.mutation_propability);
    generative_algorithm_obj.to_set_agents(p_parameters->parameters_configurations_for_optimizer_obj.agents);
    generative_algorithm_obj.to_set_new_agents(p_parameters->parameters_configurations_for_optimizer_obj.new_agents);
    generative_algorithm_obj.to_set_iterations(p_parameters->parameters_configurations_for_optimizer_obj.iterations);

    generative_algorithm_obj.to_set_fitnes_function(fitness_function_varied_reference_signal);
    generative_algorithm_obj.to_set_fitness_function_parameters(p_parameters);

    to_set_answer( generative_algorithm_obj.to_solve_array_out() );

    auto k = to_check_answer();
    p_parameters->parameters_p_objects_parameters_obj.p_regulator->to_set_koefficients(k[0], k[1], k[2]);
}

void Regulator_tuner_my_gradient_algorithm::to_tune()
{


    stochastic_gradient_method_step_based_obj.to_set_tries_amount(p_parameters->parameters_for_gradient_obj.tries);
    stochastic_gradient_method_step_based_obj.to_set_initiation_minimum(p_parameters->parameters_configurations_for_optimizer_obj.min_init);
    stochastic_gradient_method_step_based_obj.to_set_initiation_maximum(p_parameters->parameters_configurations_for_optimizer_obj.max_init);
    stochastic_gradient_method_step_based_obj.to_set_initiation_function(nullptr);

    stochastic_gradient_method_step_based_obj.to_set_step(p_parameters->parameters_for_gradient_obj.dx);
    stochastic_gradient_method_step_based_obj.to_set_learn_coefficient(p_parameters->parameters_for_gradient_obj.learn_step);

    stochastic_gradient_method_step_based_obj.to_set_iteration(p_parameters->parameters_configurations_for_optimizer_obj.iterations);
    stochastic_gradient_method_step_based_obj.to_set_fitness_function(fitness_function_varied_reference_signal);
    stochastic_gradient_method_step_based_obj.to_set_gradient_function(gradient_by_step);
    //stochastic_gradient_method_step_based_obj.to_get_fitness_function_value();

    to_set_answer( stochastic_gradient_method_step_based_obj.to_solve_array_out(p_parameters) );

    auto k = to_check_answer();

    //for ( auto & i : k ) if (1) i = 0;

    p_parameters->parameters_p_objects_parameters_obj.p_regulator->to_set_koefficients(k[0], k[1], k[2]);
}

Regulator_tuner_automated_manual_algorithm_interface::Regulator_tuner_automated_manual_algorithm_interface(parameters_for_optimizer& _parameters_to_set)
{
    p_parameters = new parameters_for_optimizer;
    to_copy_parameters_for_optimizer(*p_parameters, _parameters_to_set);
}

void Regulator_tuner_my_ziegler_nichols_method::to_tune()
{
    PID_regulator * p_regulator = p_parameters->parameters_p_objects_parameters_obj.p_regulator;
    Automated_control_system * p_acs_model = p_parameters->parameters_p_objects_parameters_obj.p_acs_model;

    to_do_first_step();
    while (!to_do_aproximating_step_Is_aproximated()) continue;
    to_do_second_step();
    to_do_third_step();
    to_do_final_step();
}

#include "experiment_executor.h"
#include "default_configuration_setter.h"
#include "container_analyzer.h"

Regulator_tuner_automated_manual_algorithm_interface::~Regulator_tuner_automated_manual_algorithm_interface() {}

void Regulator_tuner_my_ziegler_nichols_method::to_do_first_step()
{
    // DELETE
    std::cout << "firstStep_start\n" << std::flush;
    // END_DELETE
    //data
    PID_regulator * p_regulator = p_parameters->parameters_p_objects_parameters_obj.p_regulator;
    Automated_control_system * p_acs_model = p_parameters->parameters_p_objects_parameters_obj.p_acs_model;
    double t_registrare = p_parameters->parameters_for_fitness_function_obj.t_registrate;

    //data setting //Experiment_executor_velocity_record
    //std::shared_ptr<Experiment_executor_for_fitness_function> p_experiment = std::make_shared<Experiment_executor_for_fitness_function>();
    std::shared_ptr<Experiment_executor_velocity_record> p_experiment = std::make_shared<Experiment_executor_velocity_record>();
    Default_configuration_setter setter;
    setter.to_set_experiment_parameters(p_experiment.get());
    p_experiment->to_set_t_length(p_parameters->parameters_for_fitness_function_obj.length);
    p_experiment->to_get_model_to_run(p_acs_model);

    //alg
    bool cycle_must_proceed = true;
    std::vector<double> output;
    p_experiment->to_set_vector(output);
    container_analyzer analyzer;
    std::array<double, 3> & ans = to_check_answer();
    ans[0] += k_start;
    ans[1] = 0;
    ans[2] = 0;
    while(cycle_must_proceed)
    {
        k_crit_prev = ans[0];
        ans[0] += k_start;
        p_regulator->to_set_koefficients(ans[0]);
        p_experiment->to_run();
        if ( analyzer.is_oscillating(output) ) cycle_must_proceed = false;

        // DELETE
        std::cout << "firstStep_cycle\t" << ans[0] << "\n"<< std::flush;
        // END_DELETE

        output.clear();
    }
}

bool Regulator_tuner_my_ziegler_nichols_method::to_do_aproximating_step_Is_aproximated()
{
    //data
    PID_regulator * p_regulator = p_parameters->parameters_p_objects_parameters_obj.p_regulator;
    Automated_control_system * p_acs_model = p_parameters->parameters_p_objects_parameters_obj.p_acs_model;
    std::array<double, 3> & ans = to_check_answer();
    std::shared_ptr<Experiment_executor_for_fitness_function> p_experiment = std::make_shared<Experiment_executor_for_fitness_function>();
    container_analyzer analyzer;
    std::vector<double> output;
    double t_registrare = p_parameters->parameters_for_fitness_function_obj.t_registrate;

    //data setting
    Default_configuration_setter setter;
    setter.to_set_experiment_parameters(p_experiment.get());
    p_experiment->to_set_t_length(p_parameters->parameters_for_fitness_function_obj.length);
    p_experiment->to_get_model_to_run(p_acs_model);
    p_experiment->to_set_vector(output);

    //alg
    ans[0] *= 0.75;
    p_regulator->to_set_koefficients(ans[0]);
    p_experiment->to_run();

    if ( analyzer.is_oscillating(output) ) return false;

    else
    {
        output.clear();
        k_crit_prev = ans[0];

        ans[0] /= 0.75;
        ans[0] *= 0.875;
        p_regulator->to_set_koefficients(ans[0]);
        p_experiment->to_run();


        if ( !analyzer.is_oscillating(output) ) ans[0] /= 0.875;
    }
    // DELETE
    std::cout << "aproximateStep_cycle\t" << ans[0] << "\n" << std::flush;
    // END_DELETE

    return true;
}

void Regulator_tuner_my_ziegler_nichols_method::to_do_second_step()
{
    //data
    PID_regulator * p_regulator = p_parameters->parameters_p_objects_parameters_obj.p_regulator;
    Automated_control_system * p_acs_model = p_parameters->parameters_p_objects_parameters_obj.p_acs_model;
    double t_registrare = p_parameters->parameters_for_fitness_function_obj.t_registrate;

    //data setting
    std::shared_ptr<Experiment_executor_for_fitness_function> p_experiment = std::make_shared<Experiment_executor_for_fitness_function>();
    Default_configuration_setter setter;
    setter.to_set_experiment_parameters(p_experiment.get());
    p_experiment->to_set_t_length(p_parameters->parameters_for_fitness_function_obj.length);
    p_experiment->to_get_model_to_run(p_acs_model);

    //alg
    std::array<double, 3> & ans = to_check_answer();
    std::vector<double> output;
    p_experiment->to_set_vector(output);
    p_experiment->to_run();

    container_analyzer analyzer;
    ans[1] = analyzer.to_calculate_period_in_Get(output, t_registrare);
}

void Regulator_tuner_my_ziegler_nichols_method::to_do_third_step()
{
    std::array<double, 3> results = to_check_answer();
    to_set_answer( std::array<double, 3> { 0.6 * results[0] , 1.2 * results[0] / results[1], 0.125 * results[0] * results[1]} );
}

void Regulator_tuner_my_ziegler_nichols_method::to_do_final_step()
{
    PID_regulator * p_regulator = p_parameters->parameters_p_objects_parameters_obj.p_regulator;

    auto ans = to_check_answer();
    p_regulator->to_set_koefficients(ans[0], ans[1], ans[2]);
}
