#include "automated_control_system_element_interface.h"

Automated_control_system_element_interface::Automated_control_system_element_interface() : parameters(3,0)
{}

const int Automated_control_system_element_interface::to_check_the_type() const
{
    return the_type;
}
bool Automated_control_system_element_interface::to_set_dt(double const & _r_dt)
{
    double * r_pareameter = &parameters[DT];
    if (r_pareameter == nullptr) return false;
    *r_pareameter = _r_dt;
    return true;
}

bool Automated_control_system_element_interface::to_receive_input_signal(double const & _r_input_signal)
{
    double * r_pareameter = &parameters[INPUT_SIGNAL];
    if (r_pareameter == nullptr) return false;
    *r_pareameter = _r_input_signal;
    return true;
}

double Automated_control_system_element_interface::to_get_output_signal()
{
    return parameters[OUTPUT_SIGNAL];
}

bool Automated_control_system_element_interface::to_set_configurative_parameters(const std::vector<double> & _r_parameters)
{
    {
        auto i = std::begin(parameters);
        auto j = std::begin(_r_parameters);
        int k = 0;
        for (
                ;
                i != std::end(parameters) && j != std::end(_r_parameters) && k < END_INTERFACE;
                ++i, ++j, ++k
             )
            *i = *j;
        if (i == std::end(parameters) || j == std::end(_r_parameters)) return false;
    }
    return true;
}

const std::vector<double> & Automated_control_system_element_interface::to_check_parameters() const
{
    return parameters;
}

std::vector<double> & Automated_control_system_element_interface::to_get_parameters()
{
    return parameters;
}

unsigned int Automated_control_system_element_interface::to_check_amount_of_parameters() const
{
    return parameters.size();
}

Automated_control_system_element_interface::~Automated_control_system_element_interface() //virtual
{}


