#include "dc_source.h"

DC_source::DC_source()
{
    the_type = DC_source::ENERGY_SOURCE;
    while (parameters.size() < SIZE) parameters.push_back(0);
}


void DC_source::to_set_max_voltage(double _v)
{
    parameters[MAX_VOLTAGE] = _v > parameters[MIN_VOLTAGE] ? _v : parameters[MIN_VOLTAGE];
}
void DC_source::to_set_min_voltage(double _v)
{
    parameters[MIN_VOLTAGE] = _v < parameters[MAX_VOLTAGE] ? _v : parameters[MAX_VOLTAGE];
}

bool DC_source::to_verify_amount_of_parameters() const
{
    if (parameters.size() != SIZE) return false;
    return true;
}
bool DC_source::to_set_element_parameters(const std::vector<double> & _r_parameters)
{
    {
        auto i = std::begin(parameters);
        std::advance(i, END_INTERFACE);
        auto j = std::begin(_r_parameters);
        std::advance(j, END_INTERFACE);
        int k = 0;
        for (
                ;
                i != std::end(parameters) && j != std::end(_r_parameters) && k < END_SOURCE;
                ++i, ++j, ++k
             )
            *i = *j;
        if ( (i <= std::end(parameters) && j <= std::end(_r_parameters)) ) return false; // must be used to fix others
    }
    return true;
}
bool DC_source::to_set_all_parameters(const std::vector<double> & _r_parameters)
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
void DC_source::to_calculate()
{
    parameters[OUTPUT_SIGNAL] =
            (parameters[INPUT_SIGNAL] > parameters[MAX_VOLTAGE]) ?
                parameters[MAX_VOLTAGE]
            :
                (parameters[INPUT_SIGNAL] < parameters[MIN_VOLTAGE]) ?
                    parameters[MIN_VOLTAGE]
                :
                    parameters[INPUT_SIGNAL];
}
