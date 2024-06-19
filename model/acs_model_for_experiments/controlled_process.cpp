#include "controlled_process.h"

bool DC_engine::to_check_amount_of_parameters() const
{
    if (parameters.size() != SIZE) return false;
    return true;
}
DC_engine::DC_engine() : Automated_control_system_element_interface()
{}
DC_engine::~DC_engine()
{
    Automated_control_system_element_interface::~Automated_control_system_element_interface();
}

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
        if (i == std::end(parameters) || j == std::end(_r_parameters)) return false;
    }
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
        if (i == std::end(parameters) || j == std::end(_r_parameters)) return false;
    }
    return true;
}

void DC_engine::to_calculate()
{
    // THE SECOND-ORDER DERIVATIVES DEFINITION STAGE
        // to define d(current)/dt
    parameters[DCURRENT_DT] =
            (
                parameters[VOLTAGE]
                - parameters[KF] * parameters[VELOCITY]
                - parameters[CURRENT] * parameters[RESISTANCE]
            )
            / parameters[INDUCTIVITY];
    // d(current)/dt has been defined
    // to define d(velocity)/dt or d^2(theta)/dt^2
    parameters[MOMENT_OF_INERTIA] = parameters[MOMENT_OF_INERTIA_OF_ENGINE] + parameters[MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD];
    parameters[ACCELERATION] = ( parameters[TORQUE] - parameters[TORQUE_OF_LOAD] ) / parameters[MOMENT_OF_INERTIA];
    // d(velocity)/dt or d^2(theta)/dt^2 has been defined



    // THE FIRST-ORDER DERIVATIVES DEFINITION STAGE
        // to define velocity or d(theta)/dt
    parameters[VELOCITY] = parameters[ACCELERATION] * parameters[DT];
    parameters[CURRENT] = parameters[DCURRENT_DT] * parameters[DT];
    // velocity or d(theta)/dt has been defined



    // THE NULL-ORDER DERIVATIVES DEFINITION STAGE
        // to define theta
    parameters[THETA] = parameters[VELOCITY] * parameters[DT];
    // theta has been defined
    // THE NULL-ORDER ... FOR THE NEXT CALCULATION CYCLE
    // to define torque
    parameters[TORQUE] = parameters[KF] * parameters[CURRENT];
    // torque has been defined
    // do define the torque of the load
    parameters[TORQUE_OF_LOAD] =
            parameters[LOAD_K_0]
            + parameters[LOAD_K_1] * parameters[VELOCITY]
            + parameters[LOAD_K_2] * parameters[VELOCITY] * parameters[VELOCITY];
    // torque of the load has been defined

    // THE END OF THE CYCLE. THE NEXT ONE MUST BEGIN.
}
