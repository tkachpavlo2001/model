#include "controlled_process.h"

void DC_engine::to_calculate_the_load()
{
    parameters[TORQUE_OF_LOAD] =
            parameters[LOAD_K_0] +
            parameters[LOAD_K_1] * parameters[VELOCITY] +
            parameters[LOAD_K_2] * parameters[VELOCITY] * parameters[VELOCITY];
}
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
    parameters[MOMENT_OF_INERTIA] = parameters[MOMENT_OF_INERTIA_OF_ENGINE] + parameters[MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD];
    parameters[ACCELERATION] = ( parameters[TORQUE] - parameters[TORQUE_OF_LOAD] ) / parameters[MOMENT_OF_INERTIA];
    parameters[VELOCITY] = parameters[ACCELERATION] * parameters[DT];
    to_calculate_the_load();
    parameters[THETA] = parameters[VELOCITY] * parameters[DT];
}
