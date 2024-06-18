#include "controlled_process.h"

void DC_engine::to_calculate_the_load()
{
    ;
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
    parameters[ACCELERATION] = ( parameters[TORQUE] - parameters[TORQUE_RESISTING] ) / parameters[MOMENT_OF_INERTIA];
}
