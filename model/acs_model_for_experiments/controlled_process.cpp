#include "controlled_process.h"
#include<array>
#include<string>
#include<iostream>

DC_engine::DC_engine() : Automated_control_system_element_interface()
{
    while (parameters.size() < SIZE) parameters.push_back(0);
}

DC_engine::~DC_engine(){}

bool DC_engine::to_set_element_parameters(const std::vector<double> & _r_parameters)
{
    {
        auto i = std::begin(parameters);
        std::advance(i, END_INTERFACE);
        auto j = std::begin(_r_parameters);
        int k = 0;
        for (
                ;
                i != std::end(parameters) && j != std::end(_r_parameters) && k < END_DC_ENGINE;
                ++i, ++j, ++k
             )
            *i = *j;
        if ( !(i == std::end(parameters) && j == std::end(_r_parameters)) ) return false;
    }
    // THE NULL-ORDER ... FOR THE NEXT CALCULATION CYCLE
    to_define_torque_all_and_inertion(parameters);
    return true;
}

bool DC_engine::to_verify_amount_of_parameters() const
{
    if (parameters.size() != SIZE) return false;
    return true;
}

bool DC_engine::to_set_all_parameters(const std::vector<double> & _r_parameters)
{
    {
        auto i = std::begin(parameters);
        auto j = std::begin(_r_parameters);
        int k = 0;
        for (
                ;
                i != std::end(parameters) && j != std::end(_r_parameters) && k < END_DC_ENGINE;
                ++i, ++j, ++k
             )
            *i = *j;
        if ( !(i == std::end(parameters) && j == std::end(_r_parameters)) ) return false;
    }
    // THE NULL-ORDER ... FOR THE NEXT CALCULATION CYCLE
    to_define_torque_all_and_inertion(parameters);
    return true;
}

void DC_engine::to_calculate()
{
    parameters[VOLTAGE] = parameters[INPUT_SIGNAL];

    to_runge_kutta_method();

    parameters[OUTPUT_SIGNAL] = parameters[VELOCITY];

    // THE END OF THE CYCLE. THE NEXT ONE MUST BEGIN.
}
void DC_engine::to_runge_kutta_method()
{
    std::array<std::vector<double>, 4> K_parameters;

    for (auto i = std::begin(K_parameters); i < std::end(K_parameters); ++i)
    {
        *i = parameters;
        for(auto j = std::begin(*i); j < std::end(*i); ++j)
            *j = 0;
    }

    std::vector<double> doubled_parameters = parameters;
    unsigned int i = 0;
    for (auto p_i_K_parameters = std::begin(K_parameters); p_i_K_parameters < std::end(K_parameters); ++p_i_K_parameters, ++i)
    {
        auto & i_K_parameters = *p_i_K_parameters;
        auto new_parameters = doubled_parameters;

        // THE SECOND-ORDER DERIVATIVES DEFINITION STAGE
        to_define_second_order_derivatives(new_parameters);

        // THE FIRST-ORDER DERIVATIVES DEFINITION STAGE
        to_define_first_order_derivatives(new_parameters);

        // THE NULL-ORDER DERIVATIVES DEFINITION STAGE
        to_define_the_values(new_parameters);
/*
        std::vector<std::string> titles;
        titles.push_back("DT: ");
        titles.push_back("T: ");
        titles.push_back("INPUT_SIGNAL: ");
        titles.push_back("OUTPUT_SIGNAL: ");
        titles.push_back("LOAD_K_0: ");
        titles.push_back("LOAD_K_1: ");
        titles.push_back("LOAD_K_2: ");
        titles.push_back("RESISTANCE: ");
        titles.push_back("INDUCTIVITY: ");
        titles.push_back("KF: ");
        titles.push_back("MOMENT_OF_INERTIA: ");
        titles.push_back("MOMENT_OF_INERTIA_OF_ENGINE: ");
        titles.push_back("MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD: ");
        titles.push_back("THETA: ");
        titles.push_back("VELOCITY: ");
        titles.push_back("ACCELERATION: ");
        titles.push_back("COULOMBLS: ");
        titles.push_back("CURRENT: ");
        titles.push_back("DCURRENT_DT: ");
        titles.push_back("VOLTAGE: ");
        titles.push_back("TORQUE: ");
        titles.push_back("TORQUE_OF_LOAD: ");
*/
        if (i == 0)
        {
            unsigned int j = BEGIN_NONSTATIC;
            for (auto i = &(i_K_parameters[BEGIN_NONSTATIC]); i < &(i_K_parameters[END_NONSTATIC]) && j < END_NONSTATIC; ++i, ++j)
            {
                *i = new_parameters[j] - parameters[j];
                *i /= 2;
                doubled_parameters[j] += *i; // to parameters values
            }
            doubled_parameters[DT] /= 2;
/*
            std::cout << "After the first runge-kutta step the new parameters:\n";
            unsigned int k = 0;
            for (const auto i : new_parameters)
            {
                std::cout << titles[k] << '\t' << i << std::endl;
                k++;
            }
            std::cout << std::endl;

            std::cout << "After the first runge-kutta step the K_parameters[0]:\n";
            k = 0;
            for (const auto i : i_K_parameters)
            {
                std::cout << titles[k] << '\t' << i << std::endl;
                k++;
            }
            std::cout << std::endl;

            std::cout << "After the first runge-kutta step the doubled parameters:\n";
            k = 0;
            for (const auto i : doubled_parameters)
            {
                std::cout << titles[k] << '\t' << i << std::endl;
                k++;
            }
            std::cout << std::endl;
*/
        }
        if (i == 1)
        {
            unsigned int j = BEGIN_NONSTATIC;
            for (auto i = &(i_K_parameters[BEGIN_NONSTATIC]); i < &(i_K_parameters[END_NONSTATIC]) && j < END_NONSTATIC; ++i, ++j)
            {
                *i = new_parameters[j] - parameters[j];
                *i /= 2;
                doubled_parameters[j] = parameters[j];
                doubled_parameters[j] += *i; // to parameters values
            }
            doubled_parameters[DT] /= 2;
        }
        if (i == 2)
        {
            unsigned int j = BEGIN_NONSTATIC;
            for (auto i = &(i_K_parameters[BEGIN_NONSTATIC]); i < &(i_K_parameters[END_NONSTATIC]) && j < END_NONSTATIC; ++i, ++j)
            {
                *i = new_parameters[j] - parameters[j];
                *i /= 2;
                doubled_parameters[j] = parameters[j];
                doubled_parameters[j] += *i; // to parameters values
            }
            doubled_parameters[DT] /= 2;
        }
        if (i == 3)
        {
            unsigned int j = BEGIN_NONSTATIC;
            for (auto i = &(i_K_parameters[BEGIN_NONSTATIC]); i < &(i_K_parameters[END_NONSTATIC]) && j < END_NONSTATIC; ++i, ++j)
            {
                *i = new_parameters[j] - parameters[j];
            }
        }
    }

    unsigned int j = BEGIN_NONSTATIC;
    for (auto i_parameters = &(parameters[BEGIN_NONSTATIC]); i_parameters < &(parameters[END_NONSTATIC]) && j < END_NONSTATIC; ++i_parameters, ++j)
    {
        *i_parameters += (K_parameters[0][j] + 2 * K_parameters[1][j] + 2 * K_parameters[2][j] + K_parameters[3][j]) / 6;
    }
}

void DC_engine::to_define_second_order_derivatives(std::vector<double>& _parameters)
{
    // to define d(current)/dt
    to_define_dcurrent_dt(_parameters);
    // d(current)/dt has been defined
    // to define d(velocity)/dt or d^2(theta)/dt^2
    to_define_acceleration(_parameters);
    // d(velocity)/dt or d^2(theta)/dt^2 has been defined
}
void DC_engine::to_define_dcurrent_dt(std::vector<double>& _parameters)
{
    _parameters[DCURRENT_DT] =
            (
                _parameters[VOLTAGE]
                - _parameters[KF] * _parameters[VELOCITY]
                - _parameters[CURRENT] * _parameters[RESISTANCE]
            )
            / _parameters[INDUCTIVITY];
}
void DC_engine::to_define_acceleration(std::vector<double>& _parameters)
{
    to_define_torque_all_and_inertion(_parameters);
    _parameters[ACCELERATION] = ( _parameters[TORQUE] - _parameters[TORQUE_OF_LOAD] ) / _parameters[MOMENT_OF_INERTIA];
}
void DC_engine::to_define_first_order_derivatives(std::vector<double>& _parameters)
{
    // to define velocity or d(theta)/dt
    _parameters[VELOCITY] = _parameters[ACCELERATION] * _parameters[DT];
    _parameters[CURRENT] = _parameters[DCURRENT_DT] * _parameters[DT];
    // velocity or d(theta)/dt has been defined
}
void DC_engine::to_define_the_values(std::vector<double>& _parameters)
{
    // to define theta
    _parameters[THETA] = _parameters[VELOCITY] * _parameters[DT];
    // theta has been defined
    // THE NULL-ORDER ... FOR THE NEXT CALCULATION CYCLE
    to_define_torque_all_and_inertion(_parameters);

}
void DC_engine::to_define_torque_all_and_inertion(std::vector<double>& _parameters)
{
    _parameters[MOMENT_OF_INERTIA] = _parameters[MOMENT_OF_INERTIA_OF_ENGINE] + _parameters[MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD];
    // to define torque
    _parameters[TORQUE] = _parameters[KF] * _parameters[CURRENT];
    // torque has been defined
    // do define the torque of the load
    _parameters[TORQUE_OF_LOAD] =
            _parameters[LOAD_K_0]
            + _parameters[LOAD_K_1] * _parameters[VELOCITY]
            + _parameters[LOAD_K_2] * _parameters[VELOCITY] * _parameters[VELOCITY];
    // torque of the load has been defined
}
