#include "controlled_process.h"
#include<array>
#include<string>
#include<iostream>
#include<cmath>

DC_engine::DC_engine() : Automated_control_system_element_interface()
{
    the_type = DC_engine::PROCESS;
    while (parameters.size() < SIZE) parameters.push_back(0);
}

DC_engine::~DC_engine(){}

void DC_engine::to_set_calculation_mode(DC_engine::calculation_mode_states _calculation_mode_state)
{
    calculation_mode_state = _calculation_mode_state;
}
const DC_engine::calculation_mode_states DC_engine::to_check_calculation_mode()
{
    return calculation_mode_state;
}

bool DC_engine::to_set_element_parameters(const std::vector<double> & _r_parameters)
{
    {
        auto i = std::begin(parameters);
        std::advance(i, END_INTERFACE);
        auto j = std::begin(_r_parameters);
        std::advance(j, END_INTERFACE);
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

    if (calculation_mode_state == DC_engine::EULER)
        to_solve_with_euler();
    if (calculation_mode_state == DC_engine::RUNGE_KUTTA)
        to_solve_with_runge_kutta();

    parameters[OUTPUT_SIGNAL] = parameters[THETA];
}
void DC_engine_velocity_out::to_calculate()
{
    DC_engine::to_calculate();

    parameters[OUTPUT_SIGNAL] = parameters[VELOCITY];
}
void DC_engine::to_solve_with_euler()
{
    std::vector<double> new_parameters = parameters;

    new_parameters[DTORQUE_OF_LOAD_DVELOCITY] = to_dtorque_of_load_dw(
                parameters[VELOCITY],
                parameters[LOAD_K_1],
                parameters[LOAD_K_EXP_LIMIT],
                parameters[LOAD_K_EXP_CURVATURE],
                parameters[TORQUE_OF_LOAD]
                );
    //////////
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


    new_parameters[TORQUE_OF_LOAD] = to_actulize_the_torque_of_load_from_dT_dw(
                parameters[TORQUE_OF_LOAD],
                parameters[ACCELERATION],
                new_parameters[DTORQUE_OF_LOAD_DVELOCITY],
                parameters[DT]);
    new_parameters[CURRENT] = to_actulize_current(
                parameters[CURRENT],
                new_parameters[DCURRENT_DT],
                parameters[DT]
                );
    new_parameters[VELOCITY] = to_actulize_velocity(
                parameters[VELOCITY],
                new_parameters[ACCELERATION],
                parameters[DT]
                );


    new_parameters[THETA] = to_actulize_theta(
                parameters[THETA],
                new_parameters[VELOCITY],
                parameters[DT]
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

    for (auto i : {ACCELERATION, CURRENT, VELOCITY, THETA, TORQUE_OF_LOAD})
        parameters[i] += ( k_1[i] + 2 * k_2[i] + 2 * k_3[i] + k_4[i] ) / 6;

}
void DC_engine::runge_kutta_stage_1(std::vector<double>& k_1)
{
    // dy/dt = f(x,t)
    k_1[DTORQUE_OF_LOAD_DVELOCITY] = to_dtorque_of_load_dw(
                parameters[VELOCITY],
                parameters[LOAD_K_1],
                parameters[LOAD_K_EXP_LIMIT],
                parameters[LOAD_K_EXP_CURVATURE],
                parameters[TORQUE_OF_LOAD]
                );
    k_1[DCURRENT_DT] =  to_dcurrent_dt(
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
    // dy = dt * f(x,t) = dy/dt * dt = dy
    k_1[TORQUE_OF_LOAD] = parameters[DT] * k_1[ACCELERATION] * k_1[DTORQUE_OF_LOAD_DVELOCITY];
    k_1[CURRENT] = parameters[DT] * k_1[DCURRENT_DT];
    k_1[VELOCITY] = parameters[DT] * k_1[ACCELERATION];
    // ddy = dt * dt * f(<x + kx/2>, t) = dt * dy = dt * dy * dt = dt * <x + kx/2> * dt
    k_1[THETA] = parameters[DT] * k_1[VELOCITY];
}
void DC_engine::runge_kutta_stage_2(std::vector<double>& k_2,std::vector<double>& k_1)
{
    // ddx/dtt = dy/dt
    // dx/dt = dy/dt * dt = dy
    // dx = dy * dt
    // x = x0 + dx = x0 + dy * dt

    // dy/dt = f(<x + kx/2>, t)
    k_2[DTORQUE_OF_LOAD_DVELOCITY] = to_dtorque_of_load_dw(
                parameters[VELOCITY] + k_1[VELOCITY] / 2,
                parameters[LOAD_K_1],
                parameters[LOAD_K_EXP_LIMIT],
                parameters[LOAD_K_EXP_CURVATURE],
                parameters[TORQUE_OF_LOAD] + k_1[TORQUE_OF_LOAD] / 2
                );
    k_2[DCURRENT_DT] =  to_dcurrent_dt(
                parameters[VOLTAGE],
                parameters[CURRENT] + k_1[CURRENT] / 2,
                parameters[RESISTANCE],
                parameters[KF],
                parameters[VELOCITY] + k_1[VELOCITY] / 2,
                parameters[INDUCTIVITY]
                );
    k_2[ACCELERATION] = to_dvelocity_dt(
                parameters[KF],
                parameters[CURRENT] + k_1[CURRENT] / 2,
                parameters[TORQUE_OF_LOAD] + k_1[TORQUE_OF_LOAD] / 2,
                parameters[MOMENT_OF_INERTIA]
                );
    // dy = dt * f(<x + kx/2>, t) = dy/dt * dt = dy
    k_2[TORQUE_OF_LOAD] = parameters[DT] * k_2[ACCELERATION] * k_2[DTORQUE_OF_LOAD_DVELOCITY];
    k_2[CURRENT] = parameters[DT] * k_2[DCURRENT_DT];
    k_2[VELOCITY] = parameters[DT] * k_2[ACCELERATION];
    // ddy = dt * dt * f(<x + kx/2>, t) = dt * dy = dt * dy * dt = dt * <x + kx/2> * dt
    k_2[THETA] = parameters[DT] * (parameters[VELOCITY] + k_1[VELOCITY] / 2);
}
void DC_engine::runge_kutta_stage_3(std::vector<double>& k_3,std::vector<double>& k_2)
{
    // dy/dt = f(<x + kx/2>, t)
    k_3[DTORQUE_OF_LOAD_DVELOCITY] = to_dtorque_of_load_dw(
                parameters[VELOCITY] + k_2[VELOCITY] / 2,
                parameters[LOAD_K_1],
                parameters[LOAD_K_EXP_LIMIT],
                parameters[LOAD_K_EXP_CURVATURE],
                parameters[TORQUE_OF_LOAD] + k_2[TORQUE_OF_LOAD] / 2
                );
    k_3[DCURRENT_DT] =  to_dcurrent_dt(
                parameters[VOLTAGE],
                parameters[CURRENT] + k_2[CURRENT] / 2,
                parameters[RESISTANCE],
                parameters[KF],
                parameters[VELOCITY] + k_2[VELOCITY] / 2,
                parameters[INDUCTIVITY]
                );
    k_3[ACCELERATION] = to_dvelocity_dt(
                parameters[KF],
                parameters[CURRENT] + k_2[CURRENT] / 2,
                parameters[TORQUE_OF_LOAD] + k_2[TORQUE_OF_LOAD] / 2,
                parameters[MOMENT_OF_INERTIA]
                );
    // dy = dt * f(<x + kx/2>, t) = dy/dt * dt = dy
    k_3[TORQUE_OF_LOAD] = parameters[DT] * k_3[ACCELERATION] * k_3[DTORQUE_OF_LOAD_DVELOCITY];
    k_3[CURRENT] = parameters[DT] * k_3[DCURRENT_DT];
    k_3[VELOCITY] = parameters[DT] * k_3[ACCELERATION];
    // ddy = dt * dt * f(<x + kx/2>, t) = dt * dy = dt * dy * dt = dt * <x + kx/2> * dt
    k_3[THETA] = parameters[DT] * (parameters[VELOCITY] + k_2[VELOCITY] / 2);
}
void DC_engine::runge_kutta_stage_4(std::vector<double>& k_4, std::vector<double>& k_3)
{
    // dy/dt = f(<x + kx/2>, t)
    k_4[DTORQUE_OF_LOAD_DVELOCITY] = to_dtorque_of_load_dw(
                parameters[VELOCITY] + k_3[VELOCITY],
                parameters[LOAD_K_1],
                parameters[LOAD_K_EXP_LIMIT],
                parameters[LOAD_K_EXP_CURVATURE],
                parameters[TORQUE_OF_LOAD] + k_3[TORQUE_OF_LOAD]
                );
    k_4[DCURRENT_DT] =  to_dcurrent_dt(
                parameters[VOLTAGE],
                parameters[CURRENT] + k_3[CURRENT],
                parameters[RESISTANCE],
                parameters[KF],
                parameters[VELOCITY] + k_3[VELOCITY],
                parameters[INDUCTIVITY]
                );
    k_4[ACCELERATION] = to_dvelocity_dt(
                parameters[KF],
                parameters[CURRENT] + k_3[CURRENT],
                parameters[TORQUE_OF_LOAD] + k_3[TORQUE_OF_LOAD],
                parameters[MOMENT_OF_INERTIA]
                );
    // dy = dt * f(<x + kx/2>, t) = dy/dt * dt = dy
    k_4[TORQUE_OF_LOAD] = parameters[DT] * k_4[ACCELERATION] * k_4[DTORQUE_OF_LOAD_DVELOCITY];
    k_4[CURRENT] = parameters[DT] * k_4[DCURRENT_DT];
    k_4[VELOCITY] = parameters[DT] * k_4[ACCELERATION];
    // ddy = dt * dt * f(<x + kx/2>, t) = dt * dy = dt * dy * dt = dt * <x + kx/2> * dt
    k_4[THETA] = parameters[DT] * (parameters[VELOCITY] + k_3[VELOCITY]);
}
double DC_engine::to_dcurrent_dt(double U, double I, double R, double kf, double w, double L)
{
    return ( U - I * R - kf * w ) / L;
}
double DC_engine::to_dvelocity_dt(double kf, double I, double T_L, double J)
{
    return ( kf * I - T_L) / J;
}
double DC_engine::to_dtorque_of_load_dw(double w, double kL_1, double k_exp_lim, double k_exp_curv, double T)
{
    return kL_1 + (
                (w > 0)?
                    k_exp_curv *
                    k_exp_lim *
                    std::exp(-w * k_exp_curv)
                  :
                    k_exp_curv *
                    k_exp_lim *
                    std::exp(w * k_exp_curv)
                );
}
double DC_engine::to_actulize_current(double previous_I, double dI_dt, double dt)
{
    return previous_I + dI_dt * dt;
}
double DC_engine::to_actulize_velocity(double previous_w, double dw_dt, double dt)
{
    return previous_w + dw_dt * dt;
}
double DC_engine::to_actulize_theta(double previous_x, double dx_dt, double dt)
{
    return previous_x + dx_dt * dt;
}
double DC_engine::to_actulize_the_torque_of_load(double w, double kL_0, double kL_1, double k_exp_lim, double k_exp_curv)
{
    return kL_0 + kL_1 * w + ( (w > 0)? k_exp_lim * (1 - std::exp(-w * k_exp_curv)) : k_exp_lim * (- 1 + std::exp(w * k_exp_curv)));
}
double DC_engine::to_actulize_the_torque_of_load_from_dT_dw(double T, double dwdt, double dT_dw, double dt)
{
    return T + dwdt * dT_dw * dt;
}

void DC_engine::to_actulize_the_parameters()
{
    parameters[TORQUE_OF_LOAD] = to_actulize_the_torque_of_load(
                parameters[VELOCITY],
                parameters[LOAD_K_0],
                parameters[LOAD_K_1],
                parameters[LOAD_K_EXP_LIMIT],
                parameters[LOAD_K_EXP_CURVATURE]
                );

    parameters[MOMENT_OF_INERTIA] =
            parameters[MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] +
            parameters[MOMENT_OF_INERTIA_OF_ENGINE];
}
