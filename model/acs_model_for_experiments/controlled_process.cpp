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

    std::vector<double> k_1(SIZE, 0);
    std::vector<double> k_2(SIZE, 0);
    std::vector<double> k_3(SIZE, 0);
    std::vector<double> k_4(SIZE, 0);

    runge_kutta_stage_1(k_1);
    runge_kutta_stage_2(k_2,k_1);
    runge_kutta_stage_3(k_3,k_2);
    runge_kutta_stage_4(k_4,k_3);


    for (auto i : {DCURRENT_DT, ACCELERATION, CURRENT, VELOCITY, THETA})
        parameters[i] += ( k_1[i] + 2 * k_2[i] + 2 * k_3[i] + k_4[i] ) / 6;


    //parameters[COULOMBLS] += parameters[CURRENT] * parameters[DT];
    //parameters[TORQUE] = parameters[KF] * parameters[CURRENT];
}
void DC_engine::runge_kutta_stage_1(std::vector<double>& k_1)
{
    k_1[DCURRENT_DT] = parameters[DT] * to_dcurrent_dt(
                parameters[VOLTAGE],
                parameters[CURRENT],
                parameters[RESISTANCE],
                parameters[KF],
                parameters[VELOCITY],
                parameters[INDUCTIVITY]
                );
    k_1[ACCELERATION] = parameters[DT] * to_dvelocity_dt(
                parameters[KF],
                parameters[CURRENT],
                parameters[TORQUE_OF_LOAD],
                parameters[MOMENT_OF_INERTIA]
                );

    k_1[CURRENT] = parameters[DCURRENT_DT] * parameters[DT];
    k_1[VELOCITY] = parameters[ACCELERATION] * parameters[DT];
    k_1[THETA] = parameters[VELOCITY] * parameters[DT];
}
void DC_engine::runge_kutta_stage_2(std::vector<double>& k_2,std::vector<double>& k_1)
{
    k_2[DCURRENT_DT] = parameters[DT] * to_dcurrent_dt(
                parameters[VOLTAGE] + (k_1[VOLTAGE] / 2),
                parameters[CURRENT] + (k_1[CURRENT] / 2),
                parameters[RESISTANCE] + (k_1[RESISTANCE] / 2),
                parameters[KF] + (k_1[KF] / 2),
                parameters[VELOCITY] + (k_1[VELOCITY] / 2),
                parameters[INDUCTIVITY] + (k_1[INDUCTIVITY] / 2)
                );
    k_2[ACCELERATION] = parameters[DT] * to_dvelocity_dt(
                parameters[KF] + (k_1[KF] / 2),
                parameters[CURRENT] + (k_1[CURRENT] / 2),
                parameters[TORQUE_OF_LOAD] + (k_1[TORQUE_OF_LOAD] / 2),
                parameters[MOMENT_OF_INERTIA] + (k_1[MOMENT_OF_INERTIA] / 2)
                );

    k_2[CURRENT] = ( parameters[DCURRENT_DT] + k_1[DCURRENT_DT] / 2 ) * parameters[DT];
    k_2[VELOCITY] = ( parameters[ACCELERATION] + k_1[ACCELERATION] / 2 ) * parameters[DT];
    k_2[THETA] = ( parameters[VELOCITY] + k_1[VELOCITY] / 2 ) * parameters[DT];
}
void DC_engine::runge_kutta_stage_3(std::vector<double>& k_3,std::vector<double>& k_2)
{
    k_3[DCURRENT_DT] = parameters[DT] * to_dcurrent_dt(
                parameters[VOLTAGE] + (k_2[VOLTAGE] / 2),
                parameters[CURRENT] + (k_2[CURRENT] / 2),
                parameters[RESISTANCE] + (k_2[RESISTANCE] / 2),
                parameters[KF] + (k_2[KF] / 2),
                parameters[VELOCITY] + (k_2[VELOCITY] / 2),
                parameters[INDUCTIVITY] + (k_2[INDUCTIVITY] / 2)
                );
    k_3[ACCELERATION] = parameters[DT] * to_dvelocity_dt(
                parameters[KF] + (k_2[KF] / 2),
                parameters[CURRENT] + (k_2[CURRENT] / 2),
                parameters[TORQUE_OF_LOAD] + (k_2[TORQUE_OF_LOAD] / 2),
                parameters[MOMENT_OF_INERTIA] + (k_2[MOMENT_OF_INERTIA] / 2)
                );

    k_3[CURRENT] = ( parameters[DCURRENT_DT] + k_2[DCURRENT_DT] / 2 ) * parameters[DT];
    k_3[VELOCITY] = ( parameters[ACCELERATION] + k_2[ACCELERATION] / 2 ) * parameters[DT];
    k_3[THETA] = ( parameters[VELOCITY] + k_2[VELOCITY] / 2 ) * parameters[DT];
}
void DC_engine::runge_kutta_stage_4(std::vector<double>& k_4, std::vector<double>& k_3)
{
    k_4[DCURRENT_DT] = parameters[DT] * to_dcurrent_dt(
                parameters[VOLTAGE] + k_3[VOLTAGE],
                parameters[CURRENT] + k_3[CURRENT],
                parameters[RESISTANCE] + k_3[RESISTANCE],
                parameters[KF] + k_3[KF],
                parameters[VELOCITY] + k_3[VELOCITY],
                parameters[INDUCTIVITY] + k_3[INDUCTIVITY]
                );
    k_4[ACCELERATION] = parameters[DT] * to_dvelocity_dt(
                parameters[KF] + k_3[KF],
                parameters[CURRENT] + k_3[CURRENT],
                parameters[TORQUE_OF_LOAD] + k_3[TORQUE_OF_LOAD],
                parameters[MOMENT_OF_INERTIA] + k_3[MOMENT_OF_INERTIA]
                );

    k_4[CURRENT] = ( parameters[DCURRENT_DT] + k_3[DCURRENT_DT] ) * parameters[DT];
    k_4[VELOCITY] = ( parameters[ACCELERATION] + k_3[ACCELERATION] ) * parameters[DT];
    k_4[THETA] = ( parameters[VELOCITY] + k_3[VELOCITY] ) * parameters[DT];
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
