#ifndef CONTROLLED_PROCESS_H
#define CONTROLLED_PROCESS_H

#include "automated_control_system_element_interface.h"

class DC_engine_tester;

class DC_engine : public Automated_control_system_element_interface
{
private:
    friend class DC_engine_tester;
    void to_runge_kutta_method();
    void to_define_the_values(std::vector<double>&);
    void to_define_first_order_derivatives(std::vector<double>&);
    void to_define_second_order_derivatives(std::vector<double>&);
    void to_define_dcurrent_dt(std::vector<double>&);
    void to_define_acceleration(std::vector<double>&);
    void to_define_torque_all_and_inertion(std::vector<double>&);
public:
    DC_engine();
    virtual ~DC_engine() override;


    // BEGIN OF THE STATIC PARAMETERS
    // Torque of the load STATIC koefficients
    enum
    {
        BEGIN_STATIC = END_INTERFACE,       /// BE CRARE FOR THE TRUE OF THE EQUATION !!
        BEGIN_LOAD_K = END_INTERFACE,       /// BE CRARE FOR THE TRUE OF THE EQUATION !!

        // K_of_T_load
        LOAD_K_0 = BEGIN_LOAD_K + 0,
        LOAD_K_1 = BEGIN_LOAD_K + 1,
        LOAD_K_2 = BEGIN_LOAD_K + 2,


        LAST_LOAD_K = LOAD_K_2,             /// BE CRARE FOR THE TRUE OF THE EQUATION !!
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

        // I
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




        LAST_ENGINE = TORQUE_OF_LOAD,                               /// BE CRARE FOR THE TRUE OF THE EQUATION !!
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
    void to_calculate() override;
};

#endif // CONTROLLED_PROCESS_H
