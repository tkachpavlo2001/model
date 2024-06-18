#ifndef CONTROLLED_PROCESS_H
#define CONTROLLED_PROCESS_H

#include "automated_control_system_element_interface.h"

class DC_engine : public Automated_control_system_element_interface
{
private:
    enum
    {
        THETA = END_INTERFACE + 0,
        DTHETA_DT = END_INTERFACE + 1,
        VELOCITY = DTHETA_DT,
        DDTHETA_DTT = END_INTERFACE + 2,
        DVELOCITY_DT = DDTHETA_DTT,
        ACCELERATION = DVELOCITY_DT,
        COULOMBLS = END_INTERFACE + 3,
        DCOULOMBS_DT = END_INTERFACE + 4,
        CURRENT = DCOULOMBS_DT,
        DDCOULUMBS_DTT = END_INTERFACE + 5,
        DCURRENT_DT = DDCOULUMBS_DTT,
        KF = END_INTERFACE + 6,
        ELECROMOTIVE_FORCE = END_INTERFACE + 7,
        TORQUE = END_INTERFACE + 8,
        TORQUE_OF_LOAD = END_INTERFACE + 9,
        RESISTANCE = END_INTERFACE + 10,
        INDUCTIVITY = END_INTERFACE + 11,
        MOMENT_OF_INERTIA = END_INTERFACE + 12,
        MOMENT_OF_INERTIA_OF_ENGINE = END_INTERFACE + 13,
        MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD = END_INTERFACE + 14,
        LAST_ENGINE = MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD,         /// BE CRARE FOR THE TRUE OF THE EQUATION !!
        END_ENGINE = LAST_ENGINE + 1
    };
    enum
    {
        LOAD_K_0 = END_ENGINE + 0,
        LOAD_K_1 = END_ENGINE + 0,
        LOAD_K_2 = END_ENGINE + 0,
        LAST_DC_ENGINE = LOAD_K_2
    };
    void to_calculate_the_load();
    bool to_check_amount_of_parameters() const override;
protected:
    enum
    {
        END_DC_ENGINE = (LAST_DC_ENGINE + 1),
        SIZE = END_DC_ENGINE
    };
public:
    DC_engine();
    virtual ~DC_engine() override;
    bool to_set_element_parameters(const std::vector<double> &) override;
    bool to_set_all_parameters(const std::vector<double> &) override;
    void to_calculate() override;
};

#endif // CONTROLLED_PROCESS_H
