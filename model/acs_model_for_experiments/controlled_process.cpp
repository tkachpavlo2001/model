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
    to_actulize_the_parameters();
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
    to_actulize_the_parameters();
    return true;
}

void DC_engine::to_calculate()
{
    parameters[VOLTAGE] = parameters[INPUT_SIGNAL];

    to_solve();

    parameters[OUTPUT_SIGNAL] = parameters[VELOCITY];

    // THE END OF THE CYCLE. THE NEXT ONE MUST BEGIN.
}
void DC_engine::to_solve()
{
    parameters[DCURRENT_DT] = to_dcurrent_dt(
                parameters[VOLTAGE],
                parameters[CURRENT],
                parameters[RESISTANCE],
                parameters[KF],
                parameters[VELOCITY],
                parameters[INDUCTIVITY]
                );
    parameters[ACCELERATION] = to_dvelocity_dt(
                parameters[KF],
                parameters[CURRENT],
                parameters[TORQUE_OF_LOAD],
                parameters[MOMENT_OF_INERTIA]
                );

    parameters[CURRENT] += parameters[DCURRENT_DT] * parameters[DT];
    parameters[VELOCITY] += parameters[ACCELERATION] * parameters[DT];

    //parameters[COULOMBLS] += parameters[CURRENT] * parameters[DT];
    parameters[THETA] += parameters[VELOCITY] * parameters[DT];

    //parameters[TORQUE] = parameters[KF] * parameters[CURRENT];
}
double DC_engine::to_dcurrent_dt(double U, double I, double R, double kf, double w, double L)
{
    return ( U - I * R - kf * w ) / L;
}
double DC_engine::to_dvelocity_dt(double kf, double I, double T_L, double J)
{
    return ( kf * I - T_L) / J;
}
void DC_engine::to_actulize_the_parameters()
{
    parameters[TORQUE_OF_LOAD] =
            parameters[LOAD_K_0] +
            parameters[LOAD_K_1] * parameters[VELOCITY] +
            parameters[LOAD_K_2] * parameters[VELOCITY] * parameters[VELOCITY];

    parameters[MOMENT_OF_INERTIA] =
            parameters[MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] +
            parameters[MOMENT_OF_INERTIA_OF_ENGINE];
}
