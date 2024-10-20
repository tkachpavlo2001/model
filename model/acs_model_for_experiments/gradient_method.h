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
    double (*fitness_function) (double*,void*) = nullptr;
    double h = 0;
    double ITERATION = 0;
protected:
    std::array<double,POLYNOM> answer;
public:
    gradient_method_step_based();
    virtual std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > to_solve();
    std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > to_solve_and_record();
    std::array<double,POLYNOM> to_solve_array_out();
    std::array<double, POLYNOM> to_gradient(std::array<double,POLYNOM>) const;
    void to_initiate_answer(double*);
    void to_set_step(double);
    void to_set_iteration(double);
    void to_set_fitness_function(double(*)(double*,void*)=nullptr);
    //void to_set_gradient_function(double(*)(double*,void*)=nullptr);
    double to_get_fitness_function_value();
};

template <int POLYNOM>
class stochastic_gradient_method_step_based : public gradient_method_step_based<POLYNOM>
{
private:
    unsigned int tries_amount = 0;
    double init_min = 0;
    double init_max = 0;
    std::array<double, POLYNOM> (*initiation_function) () = nullptr;
    std::array<double, POLYNOM> init_funct_def();
    std::array<double, POLYNOM> call_init_funct();
public:
    stochastic_gradient_method_step_based();
    void to_set_tries_amount(unsigned int);
    void to_set_initiation_minimum(double);
    void to_set_initiation_maximum(double);
    void to_set_initiation_function(std::array<double, POLYNOM> (*) ());
    std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > to_solve() override;
};

template <int POLYNOM>
gradient_method_step_based<POLYNOM>::gradient_method_step_based()
{
    for (auto & i : answer) i = 0;
}

template <int POLYNOM>
std::array<double, POLYNOM> gradient_method_step_based<POLYNOM>::to_gradient(std::array<double,POLYNOM> _ans) const
{
    std::cerr << "MANUAL CRASH";
    std::abort();
    std::array<double, POLYNOM> answer;
    double temp = 0;
    double fx = 0;
    double fxh = 0;
    for (int i = 0; i < POLYNOM; ++i)
    {
        //fx = fitness_function(_ans.begin(),nullptr); // THERE IS CRASH
        temp = _ans[i];
        _ans[i] += h;
        //fxh = fitness_function(_ans.begin(),nullptr); // THERE IS CRASH
        answer[i] = fxh-fx;
        _ans[i] = temp;
    }
    return answer;
}

template <int POLYNOM>
std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > gradient_method_step_based<POLYNOM>::to_solve()
{
    if (fitness_function == nullptr) { std::cerr << "fitnes_function is NULL\n"; abort(); }
    std::array<double, POLYNOM> gradient;
    for (int i = 0; i < ITERATION; ++i)
    {
        gradient = to_gradient(answer);
        for(int j = 0; j < POLYNOM; ++j)
            answer[j] += gradient[j];
    }
    return std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM>> (gradient,answer);;
}

template <int POLYNOM>
std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > gradient_method_step_based<POLYNOM>::to_solve_and_record()
{
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
    return std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM>> (gradient,answer);
}

template <int POLYNOM>
std::array<double,POLYNOM> gradient_method_step_based<POLYNOM>::to_solve_array_out()
{
    std::pair<double, std::array<double,POLYNOM> > ans = this->to_solve();
    return ans.second;
}

template <int POLYNOM>
void gradient_method_step_based<POLYNOM>::to_initiate_answer(double* _init)
{
    for(int j = 0; j < POLYNOM; ++j) answer[j] = _init[j];
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
double gradient_method_step_based<POLYNOM>::to_get_fitness_function_value()
{
    std::cerr << "MANUAL CRASH";
    std::abort();
    //return fitness_function(answer.begin(),nullptr);// THERE IS CRASH
}

template <int POLYNOM>
stochastic_gradient_method_step_based<POLYNOM>::stochastic_gradient_method_step_based()
{
    srand(time(0));
    call_init_funct();
}

template <int POLYNOM>
std::array<double, POLYNOM> stochastic_gradient_method_step_based<POLYNOM>::init_funct_def()
{
    for (int i = 0; i < POLYNOM; ++i)
        this->answer[i] = myrand() * (init_max - init_min) + init_min;
}

template <int POLYNOM>
std::array<double, POLYNOM> stochastic_gradient_method_step_based<POLYNOM>::call_init_funct()
{
    if (initiation_function == nullptr) return init_funct_def();
    return initiation_function();
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
void stochastic_gradient_method_step_based<POLYNOM>::to_set_initiation_function(std::array<double, POLYNOM> (*_f) ())
{
    initiation_function = _f;
}

template <int POLYNOM>
std::pair<std::array<double,POLYNOM>, std::array<double,POLYNOM> > stochastic_gradient_method_step_based<POLYNOM>::to_solve()
{
    std::multimap<double, std::pair<std::array<double, POLYNOM>, std::array<double, POLYNOM> > > rating;
    std::pair<std::array<double, POLYNOM>, std::array<double, POLYNOM> > result;
    for (int i = 0; i < tries_amount; ++i)
    {
        call_init_funct();
        result = gradient_method_step_based<POLYNOM>::to_solve();
        rating.insert(std::pair<double, std::pair<std::array<double, POLYNOM>, std::array<double, POLYNOM> > > (this->to_get_fitness_function_value(), result) );
    }
    this->answer = rating.begin()->second.second;
    return rating.begin()->second;
}
#endif // GRADIENT_METHOD_H
