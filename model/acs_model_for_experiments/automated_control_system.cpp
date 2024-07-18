#include "automated_control_system.h"


Automated_control_system::Automated_control_system()
{
    elements.push_back(p_process);
    elements.push_back(p_regulator);
    elements.push_back(p_definder);
}

Automated_control_system::~Automated_control_system()
{

}

void Automated_control_system::to_set_dt(double _dt)
{
    for (auto i : elements) i->to_set_dt(_dt);
}

bool Automated_control_system::to_mount_the_element(Automated_control_system_element_interface * p_element)
{
    switch (p_element->to_check_the_type())
    {
    case Automated_control_system_element_interface::PROCESS: p_process = p_element;
    break;
    case Automated_control_system_element_interface::REGULATOR: p_regulator = p_element;
    break;
    case Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER: p_definder = p_element;
    break;
    default: return false;
    }
    return true;
}

bool Automated_control_system::to_mount_the_element(Automated_control_system_element_interface & r_element)
{
    return to_mount_the_element(&r_element);
}


void Automated_control_system::to_calculate() // can be paralleled
{
    p_regulator->to_receive_input_signal(p_definder->to_get_output_signal());
    p_process->to_receive_input_signal(p_regulator->to_get_output_signal());
    for(auto i : elements) i->to_calculate();
}
