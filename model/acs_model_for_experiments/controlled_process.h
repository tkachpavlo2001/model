/*
 * This file is part of ACS_MODEL.
 *
 * ACS_MODEL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACS_MODEL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACS_MODEL. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CONTROLLED_PROCESS_H
#define CONTROLLED_PROCESS_H

#include "automated_control_system_element_interface.h"

class DC_engine_tester;

class DC_engine : public Automated_control_system_element_interface
{
private:
    friend class DC_engine_tester;
    void to_actulize_the_parameters();
    double to_dcurrent_dt(double U, double I, double R, double kf, double w, double L);
    double to_dvelocity_dt(double kf, double I, double T_L, double J);
    double to_dtorque_of_load_dw(double w, double kL_1, double k_exp_lim, double k_exp_curv, double T);
    double to_actulize_the_torque_of_load(double w, double kL_0, double kL_1, double k_exp_lim, double k_exp_curv);
    double to_actulize_current(double previous_I, double dI_dt, double dt);
    double to_actulize_velocity(double previous_w, double dw_dt, double dt);
    double to_actulize_theta(double previous_x, double dx_dt, double dt);
    double to_actulize_the_torque_of_load_from_dT_dw(double T,double dwdt, double dT_dw, double dt);
    void to_solve_with_euler();
    void to_solve_with_runge_kutta();
    void runge_kutta_stage_1(std::vector<double>&);
    void runge_kutta_stage_2(std::vector<double>&,std::vector<double>&);
    void runge_kutta_stage_3(std::vector<double>&,std::vector<double>&);
    void runge_kutta_stage_4(std::vector<double>&,std::vector<double>&);
public:
    DC_engine();
    virtual ~DC_engine() override;
    enum calculation_mode_states { EULER, RUNGE_KUTTA };
private:
    calculation_mode_states calculation_mode_state;
public:
    void to_set_calculation_mode(calculation_mode_states);
    const calculation_mode_states to_check_calculation_mode();


    // BEGIN OF THE STATIC PARAMETERS
    // Torque of the load STATIC koefficients
    enum
    {
        BEGIN_STATIC = END_INTERFACE,       /// BE CRARE FOR THE TRUE OF THE EQUATION !!
        BEGIN_LOAD_K = END_INTERFACE,       /// BE CRARE FOR THE TRUE OF THE EQUATION !!

        // K_of_T_load
        LOAD_K_0 = BEGIN_LOAD_K + 0,
        LOAD_K_1 = BEGIN_LOAD_K + 1,
        LOAD_K_EXP_LIMIT = BEGIN_LOAD_K + 2,
        LOAD_K_EXP_CURVATURE = BEGIN_LOAD_K + 3,


        LAST_LOAD_K = LOAD_K_EXP_CURVATURE,             /// BE CRARE FOR THE TRUE OF THE EQUATION !!
        END_LOAD_K = LAST_LOAD_K + 1,       /// BE CRARE FOR THE TRUE OF THE EQUATION !!

    };
    // DC_engine STATIC parameters
    enum
    {
        BEGIN_DC_ENGINE = END_LOAD_K,       /// BE CRARE FOR THE TRUE OF THE EQUATION !!

        RESISTANCE = BEGIN_DC_ENGINE + 0,
        INDUCTIVITY = BEGIN_DC_ENGINE + 1,
        KF = BEGIN_DC_ENGINE + 2,

        // J
        MOMENT_OF_INERTIA = BEGIN_DC_ENGINE + 3,
        MOMENT_OF_INERTIA_OF_ENGINE = BEGIN_DC_ENGINE + 4,
        MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD = BEGIN_DC_ENGINE + 5,


        LAST_SATIC = MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD,                    /// BE CRARE FOR THE TRUE OF THE EQUATION !!
        END_STATIC = LAST_SATIC + 1,        /// BE CRARE FOR THE TRUE OF THE EQUATION !!
    };
    // END OF THE STATIC PARAMETERS

    // VARIED PARAMETERS "NONSTATIC"
    enum
    {
        BEGIN_NONSTATIC = END_STATIC,       /// BE CRARE FOR THE TRUE OF THE EQUATION !!

        // X V A
        THETA = BEGIN_NONSTATIC + 0,

        DTHETA_DT = BEGIN_NONSTATIC + 1,
        VELOCITY = DTHETA_DT,

        DDTHETA_DTT = BEGIN_NONSTATIC + 2,
        DVELOCITY_DT = DDTHETA_DTT,
        ACCELERATION = DVELOCITY_DT,

        // q I dI/dt
        COULOMBLS = BEGIN_NONSTATIC + 3,

        DCOULOMBS_DT = BEGIN_NONSTATIC + 4,
        CURRENT = DCOULOMBS_DT,

        DDCOULUMBS_DTT = BEGIN_NONSTATIC + 5,
        DCURRENT_DT = DDCOULUMBS_DTT,

        // V
        VOLTAGE = BEGIN_NONSTATIC + 6,

        // T_engine
        TORQUE = BEGIN_NONSTATIC + 7,
        TORQUE_OF_LOAD = BEGIN_NONSTATIC + 8,
        DTORQUE_OF_LOAD_DVELOCITY = BEGIN_NONSTATIC + 9,




        LAST_ENGINE = DTORQUE_OF_LOAD_DVELOCITY,                           /// BE CRARE FOR THE TRUE OF THE EQUATION !!
        END_ENGINE = LAST_ENGINE + 1,                               /// BE CRARE FOR THE TRUE OF THE EQUATION !!
    };

    // THE END OF ALL
    enum
    {
        LAST_NONSTATIC = LAST_ENGINE,                               /// BE CRARE FOR THE TRUE OF THE EQUATION !!
        END_NONSTATIC = LAST_NONSTATIC + 1,                         /// BE CRARE FOR THE TRUE OF THE EQUATION !!

        LAST_DC_ENGINE = LAST_ENGINE,                               /// BE CRARE FOR THE TRUE OF THE EQUATION !!
        END_DC_ENGINE = LAST_DC_ENGINE + 1,                         /// BE CRARE FOR THE TRUE OF THE EQUATION !!

        SIZE = END_DC_ENGINE                                        /// BE CRARE FOR THE TRUE OF THE EQUATION !!
    };

    bool to_verify_amount_of_parameters() const override;
    bool to_set_element_parameters(const std::vector<double> &) override;
    bool to_set_all_parameters(const std::vector<double> &) override;
    virtual void to_calculate() override;
};

class DC_engine_velocity_out : public DC_engine
{
    void to_calculate() override;
};

#endif // CONTROLLED_PROCESS_H
