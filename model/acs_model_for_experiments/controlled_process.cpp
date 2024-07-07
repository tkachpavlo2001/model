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

    to_solve_with_runge_kutta();

    parameters[OUTPUT_SIGNAL] = parameters[VELOCITY];

    // THE END OF THE CYCLE. THE NEXT ONE MUST BEGIN.
}
void DC_engine::to_solve_with_euler()
{
    std::vector<double> new_parameters = parameters;
    new_parameters[DCURRENT_DT] = to_dcurrent_dt(
                parameters[VOLTAGE],
                parameters[CURRENT],
                parameters[RESISTANCE],
                parameters[KF],
                parameters[VELOCITY],
                parameters[INDUCTIVITY]
                );
    new_parameters[ACCELERATION] = to_dvelocity_dt(
                parameters[KF],
                parameters[CURRENT],
                parameters[TORQUE_OF_LOAD],
                parameters[MOMENT_OF_INERTIA]
                );

    new_parameters[CURRENT] += new_parameters[DCURRENT_DT] * new_parameters[DT];
    new_parameters[VELOCITY] += new_parameters[ACCELERATION] * new_parameters[DT];

    new_parameters[THETA] += new_parameters[VELOCITY] * new_parameters[DT];
    new_parameters[TORQUE_OF_LOAD] = to_actulize_the_torque_of_load(
                new_parameters[VELOCITY],
                new_parameters[LOAD_K_0],
                new_parameters[LOAD_K_1],
                new_parameters[LOAD_K_2]
                );

    parameters = new_parameters;
}
void DC_engine::to_solve_with_runge_kutta()
{

    std::vector<double> k_1 = parameters;
    std::vector<double> k_2 = parameters;
    std::vector<double> k_3 = parameters;
    std::vector<double> k_4 = parameters;

    runge_kutta_stage_1(k_1);
    runge_kutta_stage_2(k_2,k_1);
    runge_kutta_stage_3(k_3,k_2);
    runge_kutta_stage_4(k_4,k_3);


    for (auto i : {DCURRENT_DT, ACCELERATION, CURRENT, VELOCITY, THETA}) //, TORQUE_OF_LOAD})
        parameters[i] += ( k_1[i] + 2 * k_2[i] + 2 * k_3[i] + k_4[i] ) / 6;


    parameters[TORQUE_OF_LOAD] = to_actulize_the_torque_of_load(
                parameters[VELOCITY],
                parameters[LOAD_K_0],
                parameters[LOAD_K_1],
                parameters[LOAD_K_2]
                );

    //parameters[COULOMBLS] += parameters[CURRENT] * parameters[DT];
    //parameters[TORQUE] = parameters[KF] * parameters[CURRENT];
}
void DC_engine::runge_kutta_stage_1(std::vector<double>& k_1)
{
    k_1[DCURRENT_DT] = to_dcurrent_dt(
                parameters[VOLTAGE],
                parameters[CURRENT],
                parameters[RESISTANCE],
                parameters[KF],
                parameters[VELOCITY],
                parameters[INDUCTIVITY]
                );
    k_1[ACCELERATION] = to_dvelocity_dt(
                parameters[KF],
                parameters[CURRENT],
                parameters[TORQUE_OF_LOAD],
                parameters[MOMENT_OF_INERTIA]
                );

    k_1[CURRENT] = k_1[DCURRENT_DT] * k_1[DT];
    k_1[VELOCITY] = k_1[ACCELERATION] * k_1[DT];
    k_1[THETA] = k_1[VELOCITY] * k_1[DT];

    k_1[DTORQUE_OF_LOAD_DT] = to_dtorque_of_load_dt(
                k_1[VELOCITY],
                k_1[LOAD_K_0],
                k_1[LOAD_K_1],
                k_1[LOAD_K_2],
                k_1[TORQUE_OF_LOAD]
                );
    k_1[TORQUE_OF_LOAD] = k_1[DTORQUE_OF_LOAD_DT] * k_1[DT];
}
void DC_engine::runge_kutta_stage_2(std::vector<double>& k_2,std::vector<double>& k_1)
{
    k_2[DCURRENT_DT] = to_dcurrent_dt(
                parameters[VOLTAGE] + (k_1[VOLTAGE] / 2),
                parameters[CURRENT] + (k_1[CURRENT] / 2),
                parameters[RESISTANCE] + (k_1[RESISTANCE] / 2),
                parameters[KF] + (k_1[KF] / 2),
                parameters[VELOCITY] + (k_1[VELOCITY] / 2),
                parameters[INDUCTIVITY] + (k_1[INDUCTIVITY] / 2)
                );
    k_2[ACCELERATION] = to_dvelocity_dt(
                parameters[KF] + (k_1[KF] / 2),
                parameters[CURRENT] + (k_1[CURRENT] / 2),
                parameters[TORQUE_OF_LOAD] + (k_1[TORQUE_OF_LOAD] / 2),
                parameters[MOMENT_OF_INERTIA] + (k_1[MOMENT_OF_INERTIA] / 2)
                );

    k_2[CURRENT] = ( parameters[DCURRENT_DT] + k_2[DCURRENT_DT] / 2 ) * parameters[DT];
    k_2[VELOCITY] = ( parameters[ACCELERATION] + k_2[ACCELERATION] / 2 ) * parameters[DT];
    k_2[THETA] = ( parameters[VELOCITY] + k_2[VELOCITY] / 2 ) * parameters[DT];

    k_2[DTORQUE_OF_LOAD_DT] = to_dtorque_of_load_dt(
                parameters[VELOCITY] + (k_1[VELOCITY] / 2) ,
                parameters[LOAD_K_0],
                parameters[LOAD_K_1],
                parameters[LOAD_K_2],
                parameters[TORQUE_OF_LOAD] + ( k_1[DTORQUE_OF_LOAD_DT] * k_1[DT] / 2 )
                );
    k_2[TORQUE_OF_LOAD] = k_2[DTORQUE_OF_LOAD_DT] * k_2[DT];
}
void DC_engine::runge_kutta_stage_3(std::vector<double>& k_3,std::vector<double>& k_2)
{
    k_3[DCURRENT_DT] = to_dcurrent_dt(
                parameters[VOLTAGE] + (k_2[VOLTAGE] / 2),
                parameters[CURRENT] + (k_2[CURRENT] / 2),
                parameters[RESISTANCE] + (k_2[RESISTANCE] / 2),
                parameters[KF] + (k_2[KF] / 2),
                parameters[VELOCITY] + (k_2[VELOCITY] / 2),
                parameters[INDUCTIVITY] + (k_2[INDUCTIVITY] / 2)
                );
    k_3[ACCELERATION] = to_dvelocity_dt(
                parameters[KF] + (k_2[KF] / 2),
                parameters[CURRENT] + (k_2[CURRENT] / 2),
                parameters[TORQUE_OF_LOAD] + (k_2[TORQUE_OF_LOAD] / 2),
                parameters[MOMENT_OF_INERTIA] + (k_2[MOMENT_OF_INERTIA] / 2)
                );

    k_3[CURRENT] = ( parameters[DCURRENT_DT] + k_3[DCURRENT_DT] / 2 ) * parameters[DT];
    k_3[VELOCITY] = ( parameters[ACCELERATION] + k_3[ACCELERATION] / 2 ) * parameters[DT];
    k_3[THETA] = ( parameters[VELOCITY] + k_3[VELOCITY] / 2 ) * parameters[DT];

    k_3[DTORQUE_OF_LOAD_DT] = to_dtorque_of_load_dt(
                parameters[VELOCITY] + (k_2[VELOCITY] / 2) ,
                parameters[LOAD_K_0],
                parameters[LOAD_K_1],
                parameters[LOAD_K_2],
                parameters[TORQUE_OF_LOAD] + ( k_2[DTORQUE_OF_LOAD_DT] * k_2[DT] / 2 )
                );
    k_3[TORQUE_OF_LOAD] = k_3[DTORQUE_OF_LOAD_DT] * k_3[DT];
}
void DC_engine::runge_kutta_stage_4(std::vector<double>& k_4, std::vector<double>& k_3)
{
    k_4[DCURRENT_DT] = to_dcurrent_dt(
                parameters[VOLTAGE] + k_3[VOLTAGE],
                parameters[CURRENT] + k_3[CURRENT],
                parameters[RESISTANCE] + k_3[RESISTANCE],
                parameters[KF] + k_3[KF],
                parameters[VELOCITY] + k_3[VELOCITY],
                parameters[INDUCTIVITY] + k_3[INDUCTIVITY]
                );
    k_4[ACCELERATION] = to_dvelocity_dt(
                parameters[KF] + k_3[KF],
                parameters[CURRENT] + k_3[CURRENT],
                parameters[TORQUE_OF_LOAD] + k_3[TORQUE_OF_LOAD],
                parameters[MOMENT_OF_INERTIA] + k_3[MOMENT_OF_INERTIA]
                );

    k_4[CURRENT] = ( parameters[DCURRENT_DT] + k_4[DCURRENT_DT] ) * parameters[DT];
    k_4[VELOCITY] = ( parameters[ACCELERATION] + k_4[ACCELERATION] ) * parameters[DT];
    k_4[THETA] = ( parameters[VELOCITY] + k_4[VELOCITY] ) * parameters[DT];

    k_4[DTORQUE_OF_LOAD_DT] = to_dtorque_of_load_dt(
                parameters[VELOCITY] + k_3[VELOCITY],
                parameters[LOAD_K_0],
                parameters[LOAD_K_1],
                parameters[LOAD_K_2],
                parameters[TORQUE_OF_LOAD] + k_3[DTORQUE_OF_LOAD_DT] * k_3[DT]
                );
    k_4[TORQUE_OF_LOAD] = k_4[DTORQUE_OF_LOAD_DT] * k_4[DT];
}
double DC_engine::to_dcurrent_dt(double U, double I, double R, double kf, double w, double L)
{
    return ( U - I * R - kf * w ) / L;
}
double DC_engine::to_dvelocity_dt(double kf, double I, double T_L, double J)
{
    return ( kf * I - T_L) / J;
}
double DC_engine::to_dtorque_of_load_dt(double w, double kL_0, double kL_1, double kL_2, double T)
{
    return to_actulize_the_torque_of_load(w, kL_0, kL_1, kL_2) - T;
}
double DC_engine::to_actulize_the_torque_of_load(double w, double kL_0, double kL_1, double kL_2)
{
    return kL_0 + kL_1 * w + kL_2 * w * w;
}
void DC_engine::to_actulize_the_parameters()
{
    parameters[TORQUE_OF_LOAD] = to_actulize_the_torque_of_load(
                parameters[VELOCITY],
                parameters[LOAD_K_0],
                parameters[LOAD_K_1],
                parameters[LOAD_K_2]
                );

    parameters[MOMENT_OF_INERTIA] =
            parameters[MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] +
            parameters[MOMENT_OF_INERTIA_OF_ENGINE];
}
