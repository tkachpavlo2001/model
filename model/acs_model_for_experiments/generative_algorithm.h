#ifndef GENERATIVE_ALGORITHM_H
#define GENERATIVE_ALGORITHM_H

#include<iostream>
#include<vector>
#include<ctime>
#include<random>
#include<array>
#include<map>
#include<array>

#include"tkachpavlo2001lib/tkachpavlo2001lib.hpp"

template <int POLYNOM>
class generative_algorithm
{
private:
    void add_answer(std::multimap<double, std::array<double,POLYNOM>> & _rating, const std::array<double,POLYNOM> & _answer);
    void to_initialize(std::multimap<double, std::array<double,POLYNOM>> & _answer_rating);
    void to_mutate(std::array<double, POLYNOM> & _answer);
    void to_pair(std::array<double,3> & new_agent, const std::array<double, POLYNOM> & arr_1, const std::array<double, POLYNOM> & arr_2);
    void to_make_new_generation(std::vector<std::array<double,POLYNOM>> & _new_generation, const std::multimap<double, std::array<double,POLYNOM>> & old_generation);
    void to_mutate_new_generation(std::vector<std::array<double, POLYNOM>> & _new_generation);
    void to_rate(std::multimap<double, std::array<double,POLYNOM>> & _answer_rating, const std::vector<std::array<double,POLYNOM>> & _new_generation);
    void to_cut(std::multimap<double, std::array<double,POLYNOM>> & _answer_rating);
    void to_show(const std::pair<double, std::array<double, POLYNOM>> & _answer_to_show);
    void to_show(const std::array<double, POLYNOM> & _answer_to_show);
    void to_show_iteration_status(const std::pair<double, std::array<double, POLYNOM>> & _best_answer, const std::pair<double, std::array<double, POLYNOM>> & _new_answer, int iteration);
    double MUTATION_PROBABILITY = 0.1;
    double MUTATION_STEP = 0.1;
    double MAX_INIT = 1;
    double MIN_INIT = 0;
    double AGENTS = 1000;
    double NEW_AGENTS = 3000;
    double ITERATIONS = 100;
    double (*fitnes_function) (double * answer, void *) = nullptr;
    void * fitness_function_parameters = nullptr;

public:
    generative_algorithm();
    void to_solve_record_mode();
    std::pair<double, std::array<double,POLYNOM>> to_solve();
    std::array<double,POLYNOM> to_solve_array_out();
    void to_set_min_init(double = 0);
    void to_set_max_init(double = 1);
    void to_set_mutation_step(double = 0.1);
    void to_set_mutation_propability(double = 0.1);
    void to_set_agents(double = 1000);
    void to_set_new_agents(double = 3000);
    void to_set_iterations(double = 100);
    void to_set_fitnes_function( double (*f) (double*,void*));
    void to_set_fitness_function_parameters (void*);
};

template <int POLYNOM>
generative_algorithm<POLYNOM>::generative_algorithm()
{
    to_set_min_init();
    to_set_max_init();
    to_set_mutation_step();
    to_set_mutation_propability();
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_initialize(std::multimap<double, std::array<double,POLYNOM>> & _answer_rating)
{
    for (unsigned int i = 0; i < AGENTS; i++)
    {
        std::array<double, POLYNOM> _answer;
        for (int i = 0; i < POLYNOM; i++)
        {
            _answer[i] = myrand() * (MAX_INIT - MIN_INIT) + MIN_INIT;
        }
        add_answer(_answer_rating, _answer);
    }
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::add_answer(std::multimap<double, std::array<double,POLYNOM>> & _rating, const std::array<double,POLYNOM> & _answer)
{
    double array[POLYNOM];
    for (int i = 0; i < POLYNOM; ++i)
        array[i] = _answer[i];
    double fi = fitnes_function(array, fitness_function_parameters);
    std::cout << "." << std::flush;                                                         //REDUNDANT
    _rating.insert(std::pair(fi, _answer));
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_mutate(std::array<double, POLYNOM> & _answer)
{
    for (auto i = std::begin(_answer); i != std::end(_answer); ++i)
    {
        if ( myrand() >= MUTATION_PROBABILITY ) *i += (rand() % 2 - 1) * myrand() * MUTATION_STEP;
    }
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_pair(std::array<double,3> & new_agent, const std::array<double, POLYNOM> & arr_1, const std::array<double, POLYNOM> & arr_2)
{
    for (long unsigned int i = 0; i < new_agent.size(); ++i)
        if (myrand() > 0.5)
            new_agent[i] = arr_1[i];
        else
            new_agent[i] = arr_2[i];
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_make_new_generation(std::vector<std::array<double,POLYNOM>> & _new_generation, const std::multimap<double, std::array<double,POLYNOM>> & old_generation)
{
    _new_generation.clear();
    int i = 0;
    if (!old_generation.empty()) for (auto j = std::begin(old_generation); i < NEW_AGENTS; ++i, std::advance(j, 1))
    {
        _new_generation.emplace_back();
        auto j_2 = j;
        std::advance(j_2, 1);
        if (j_2 == std::end(old_generation) )
        {
            j = std::begin(old_generation);
            j_2 = j;
            std::advance(j_2, 1);
        }
        if (j_2 != old_generation.end() && j != old_generation.end()) to_pair(_new_generation.at(i), j->second, j_2->second);
    }
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_mutate_new_generation(std::vector<std::array<double, POLYNOM>> & _new_generation)
{
    for (auto i = std::begin(_new_generation); i != std::end(_new_generation); ++i)
        to_mutate(*i);
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_rate(std::multimap<double, std::array<double,POLYNOM>> & _answer_rating, const std::vector<std::array<double,POLYNOM>> & _new_generation)
{
    _answer_rating.clear();
    if (!_new_generation.empty()) for (auto i : _new_generation)
        add_answer(_answer_rating, i);
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_cut(std::multimap<double, std::array<double,POLYNOM>> & _answer_rating)
{
    while (_answer_rating.size() > AGENTS)
    {
        _answer_rating.erase(--(std::rbegin(_answer_rating).base()));
    }
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_show(const std::pair<double, std::array<double, POLYNOM>> & _answer_to_show)
{
    std::cout << _answer_to_show.first;
    for (auto i : _answer_to_show.second) std::cout << '\t' << i;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_show(const std::array<double, POLYNOM> & _answer_to_show)
{
    std::cout << "NODATA";
    for (auto i : _answer_to_show) std::cout << '\t' << i;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_show_iteration_status(const std::pair<double, std::array<double, POLYNOM>> & _best_answer, const std::pair<double, std::array<double, POLYNOM>> & _new_answer, int iteration)
{
    std::cout << iteration << ":\nglobaly best:\t\t" << _best_answer.first;
    for (auto i : _best_answer.second) std::cout << "\t\t" << i;
    std::cout << iteration << "\nlocally best:\t\t" << _new_answer.first;
    for (auto i : _new_answer.second) std::cout << "\t\t" << i;
    std::cout << iteration << "\n";
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_solve_record_mode()
{
    if (fitnes_function == nullptr) { std::cerr << "fitness_function is NULL\n"; return; }
    if (fitness_function_parameters == nullptr) { std::cerr << "fitness_function_parameters is NULL\n"; return; }
    std::vector<std::array<double,POLYNOM>> answer_array;
    std::multimap<double, std::array<double,POLYNOM>> answer_rating;
    std::pair<double, std::array<double,POLYNOM>> answer;
    std::pair<double, std::array<double,POLYNOM>> best_answer;
    srand(time(0));
    to_initialize(answer_rating);
    answer = *std::begin(answer_rating);
    std::cout << "Initializing:\n";
    to_show(answer);
    std::cout << '\n';
    best_answer = answer;
    for ( int i = 0 ; i < ITERATIONS; ++i)
    {
        to_make_new_generation(answer_array,answer_rating);
        to_mutate_new_generation(answer_array);
        to_rate(answer_rating, answer_array);
        to_cut(answer_rating);
        answer = *(std::begin(answer_rating));
        if (best_answer.first > answer.first) best_answer = answer;
        to_show_iteration_status(best_answer, answer, i);
        std::cout << std::endl;
    }
    std::cout << "\n\nFinal answer: ";
    to_show(best_answer);
    std::cout << std::endl;
}

template <int POLYNOM>
std::pair<double, std::array<double,POLYNOM>> generative_algorithm<POLYNOM>::to_solve()
{
    if (fitnes_function == nullptr) { std::cerr << "fitnes_function is NULL\n"; std::abort(); }
    if (fitness_function_parameters == nullptr) { std::cerr << "fitness_function_parameters is NULL\n"; std::abort(); }
    std::vector<std::array<double,POLYNOM>> answer_array;
    std::multimap<double, std::array<double,POLYNOM>> answer_rating;
    std::pair<double, std::array<double,POLYNOM>> answer;
    std::pair<double, std::array<double,POLYNOM>> best_answer;
    srand(time(0));
    to_initialize(answer_rating);
    answer = *std::begin(answer_rating);
    best_answer = answer;
    for ( int i = 0 ; i < ITERATIONS; ++i)
    {
        to_make_new_generation(answer_array,answer_rating);
        to_mutate_new_generation(answer_array);
        to_rate(answer_rating, answer_array);
        to_cut(answer_rating);
        answer = *(std::begin(answer_rating));
        if (best_answer.first > answer.first) best_answer = answer;

        std::cout << "\n";                                                                        // REDUNDANT
        to_show_iteration_status(best_answer, answer, i);
        std::cout << std::endl;
    }
    return best_answer;
}

template <int POLYNOM>
std::array<double,POLYNOM> generative_algorithm<POLYNOM>::to_solve_array_out()
{
    auto answer = this->to_solve();
    return answer.second;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_set_min_init(double num)
{
    MIN_INIT = num;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_set_max_init(double num)
{
    MAX_INIT = num;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_set_mutation_step(double num)
{
    MUTATION_STEP = num;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_set_mutation_propability(double num)
{
    MUTATION_PROBABILITY = num;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_set_agents(double num)
{
    AGENTS = num;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_set_new_agents(double num)
{
    NEW_AGENTS = num;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_set_iterations(double num)
{
    ITERATIONS = num;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_set_fitnes_function( double (*f) (double*,void*))
{
    fitnes_function = f;
}

template <int POLYNOM>
void generative_algorithm<POLYNOM>::to_set_fitness_function_parameters(void* _parameters)
{
    fitness_function_parameters = _parameters;
}

#endif // GENERATIVE_ALGORITHM_H
