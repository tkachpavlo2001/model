#ifndef GRADIENT_METHOD_H
#define GRADIENT_METHOD_H

#include<array>
#include<iostream>
#include<map>
#include<random>
#include<time.h>


#include"tkachpavlo2001lib/myRand.h"



template <int POLYNOM>
class gradient_method_step_based
{
private:
    std::multimap<double, std::array<double, POLYNOM>> ans_rating;
    double (*fitness_function) (double*,void*) = nullptr;
    std::pair<double, std::array<double, POLYNOM>> (*gradient_function) (double*,void*) = nullptr;
    void (*initiation_function) (double*,void*) = nullptr;
    double h = 0;
    double learn_coefficient = 0;
    double ITERATION = 0;
    void to_initiate_answer(double*,void *);
protected:
    std::array<double,POLYNOM> answer;
public:
    gradient_method_step_based();
    virtual std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > to_solve(void*);
    std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > to_solve_and_record(void * param);
    std::array<double,POLYNOM> to_solve_array_out(void * param);
    std::array<double, POLYNOM> to_gradient(std::array<double,POLYNOM>, void*);
    void to_set_step(double);
    void to_set_iteration(double);
    void to_set_fitness_function(double(*)(double*,void*)=nullptr);
    void to_set_gradient_function(std::pair<double, std::array<double, POLYNOM>>(*)(double*,void*)=nullptr);
    void to_set_initiation_function(void(*_arg)(double*,void*)) {initiation_function=_arg;}
    //void to_set_gradient_function(double(*)(double*,void*)=nullptr);
    double to_get_fitness_function_value(void*);
    void to_set_learn_coefficient(double _arg) { learn_coefficient = _arg; }
};

template <int POLYNOM>
class stochastic_gradient_method_step_based : public gradient_method_step_based<POLYNOM>
{
private:
    unsigned int tries_amount = 0;
    double init_min = 0;
    double init_max = 0;
    std::array<double, POLYNOM> init_funct_def();
    std::array<double, POLYNOM> call_init_funct();
public:
    stochastic_gradient_method_step_based();
    void to_set_tries_amount(unsigned int);
    void to_set_initiation_minimum(double);
    void to_set_initiation_maximum(double);
    std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > to_solve(void*) override;
};

template <int POLYNOM>
gradient_method_step_based<POLYNOM>::gradient_method_step_based()
{
    for (auto & i : answer) i = 0;
}

template <int POLYNOM>
std::array<double, POLYNOM> gradient_method_step_based<POLYNOM>::to_gradient(std::array<double,POLYNOM> _ans, void * param)
{
    std::pair<double, std::array<double, POLYNOM>> gradient = gradient_function(_ans.begin(), param);
    for(int j = 0; j < POLYNOM; ++j)
        answer[j] -= gradient.second[j] * learn_coefficient;
    ans_rating.insert(std::pair<double, std::array<double,3>> (gradient.first, answer));
    return gradient.second;
}

template <int POLYNOM>
std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > gradient_method_step_based<POLYNOM>::to_solve(void * param)
{
    to_initiate_answer(answer.begin(), param);
    for (auto i : answer) std::cout << i << "\t";       // REDUNDANCE
    std::cout << std::endl << std::flush;               // REDUNDANCE
    if (fitness_function == nullptr) { std::cerr << "fitnes_function is NULL\n"; abort(); }
    std::array<double, POLYNOM> gradient;
    for (int i = 0; i < ITERATION; ++i)
    {
        std::cout << "\nNEXT_STEP:\n";                                                 // REDUNDANCE
        gradient = this->to_gradient(answer, param);
        std::cout << "\ngrad:\t"; for (auto i : gradient) std::cout << i << "\t\t";     // REDUNDANCE
        std::cout << "\nans:\t"; for (auto i : answer) std::cout << i << "\t\t";       // REDUNDANCE
        std::cout << std::endl << std::flush;                                         // REDUNDANCE
    }
    answer = (*ans_rating.begin()).second;
    return std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM>> (gradient, (*ans_rating.begin()).second);
}

template <int POLYNOM>
std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > gradient_method_step_based<POLYNOM>::to_solve_and_record(void * param)
{
    to_initiate_answer(answer.begin());
    if (fitness_function == nullptr) { std::cerr << "fitnes_function is NULL\n"; abort(); }
    std::array<double, POLYNOM> gradient;
    for (int i = 0; i < ITERATION; ++i)
    {
        std::cout << i << ":\t";
        gradient = to_gradient(answer);
        for(int j = 0; j < POLYNOM; ++j)
            std::cout << gradient[j] << "\t\t";
        std::cout << "||";
        for(int j = 0; j < POLYNOM; ++j)
            std::cout << (answer[j] -= gradient[j] * h) << "\t\t";
        std::cout << std::endl;
    }
    return std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM>> (gradient);
}

template <int POLYNOM>
std::array<double,POLYNOM> gradient_method_step_based<POLYNOM>::to_solve_array_out(void * param)
{
    std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > ans = this->to_solve(param);
    return ans.second;
}

template <int POLYNOM>
void gradient_method_step_based<POLYNOM>::to_initiate_answer(double* _init, void * param)
{
    if (initiation_function != nullptr) initiation_function(_init, param);
    else for(int j = 0; j < POLYNOM; ++j) answer[j] = 0;
}

template <int POLYNOM>
void gradient_method_step_based<POLYNOM>::to_set_step(double _h)
{
    h = _h;
}

template <int POLYNOM>
void gradient_method_step_based<POLYNOM>::to_set_iteration(double _iteration)
{
    ITERATION = _iteration;
}

template <int POLYNOM>
void gradient_method_step_based<POLYNOM>::to_set_fitness_function(double(*_f)(double*,void*))
{
    fitness_function = _f;
}


template <int POLYNOM>
void gradient_method_step_based<POLYNOM>::to_set_gradient_function(std::pair<double, std::array<double, POLYNOM>>(*_f)(double*,void*))
{
    gradient_function = _f;
}


template <int POLYNOM>
double gradient_method_step_based<POLYNOM>::to_get_fitness_function_value(void* param)
{
    return fitness_function(answer.begin(), param);
}

template <int POLYNOM>
stochastic_gradient_method_step_based<POLYNOM>::stochastic_gradient_method_step_based()
{
    srand(time(0));
}

template <int POLYNOM>
std::array<double, POLYNOM> stochastic_gradient_method_step_based<POLYNOM>::init_funct_def()
{
    for (int i = 0; i < POLYNOM; ++i)
        this->answer[i] = myrand() * (init_max - init_min) + init_min;
    return this->answer;
}



template <int POLYNOM>
void stochastic_gradient_method_step_based<POLYNOM>::to_set_tries_amount(unsigned int _n)
{
    tries_amount = _n;
}

template <int POLYNOM>
void stochastic_gradient_method_step_based<POLYNOM>::to_set_initiation_minimum(double _num)
{
    init_min = _num;
}

template <int POLYNOM>
void stochastic_gradient_method_step_based<POLYNOM>::to_set_initiation_maximum(double _num)
{
    init_max = _num;
}

template <int POLYNOM>
std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > stochastic_gradient_method_step_based<POLYNOM>::to_solve(void*param)
{
    std::multimap<double, std::pair<std::array<double, POLYNOM>, std::array<double, POLYNOM> > > rating;
    std::pair<std::array<double, POLYNOM>, std::array<double, POLYNOM> > result;
    for (int i = 0; i < tries_amount; ++i)
    {
        result = gradient_method_step_based<POLYNOM>::to_solve(param);
        rating.insert(std::pair<double, std::pair<std::array<double, POLYNOM>, std::array<double, POLYNOM> > > (this->to_get_fitness_function_value(param), result) );
    }
    this->answer = rating.begin()->second.second;
    return rating.begin()->second;
}
#endif // GRADIENT_METHOD_H
