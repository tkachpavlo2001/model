#include "reference_signal_definder_static.h"

Reference_signal_definder_static::Reference_signal_definder_static(double r) : Automated_control_system_element_interface()
{
    the_type = Reference_signal_definder_static::REFERENCE_SIGNAL_DEFINDER;
    while (parameters.size() < SIZE) parameters.push_back(0);
    to_set_signal(r);
}

void Reference_signal_definder_static::to_set_signal(double r)
{
    parameters[OUTPUT_SIGNAL] = r;
}

bool Reference_signal_definder_static::to_verify_amount_of_parameters() const
{
    if (parameters.size() != SIZE) return false;
    return true;
}

bool Reference_signal_definder_static::to_set_element_parameters(const std::vector<double> & _r_parameters)
{
    return false;
}

bool Reference_signal_definder_static::to_set_all_parameters(const std::vector<double> & _r_parameters)
{
    {
        auto i = std::begin(parameters);
        auto j = std::begin(_r_parameters);
        int k = 0;
        for (
                ;
                i != std::end(parameters) && j != std::end(_r_parameters) && k < SIZE;
                ++i, ++j, ++k
             )
            *i = *j;
        if ( !(i == std::end(parameters) && j == std::end(_r_parameters)) ) return false;
    }
    return true;
}

void Reference_signal_definder_static::to_calculate()
{}
