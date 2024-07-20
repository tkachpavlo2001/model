#include "automated_control_system.h"

#include<iostream>


void Automated_control_system::reset_vector_of_elements(Automated_control_system_element_interface * p_new_element)
{
    if(p_new_element != nullptr)
        for(auto & i : elements)
        {
            if(i->to_check_the_type() == Automated_control_system_element_interface::PROCESS)
                if(p_new_element->to_check_the_type() == Automated_control_system_element_interface::PROCESS)
                    i = p_new_element;
            if(i->to_check_the_type() == Automated_control_system_element_interface::REGULATOR)
                if(p_new_element->to_check_the_type() == Automated_control_system_element_interface::REGULATOR)
                    i = p_new_element;
            if(i->to_check_the_type() == Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER)
                if(p_new_element->to_check_the_type() == Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER)
                    i = p_new_element;
            if(i->to_check_the_type() == Automated_control_system_element_interface::ENERGY_SOURCE)
                if(p_new_element->to_check_the_type() == Automated_control_system_element_interface::ENERGY_SOURCE)
                    i = p_new_element;
        }
    else
    {
        elements.push_back(p_process);
        elements.push_back(p_regulator);
        elements.push_back(p_definder);
        elements.push_back(p_source);
    }
}

Automated_control_system::Automated_control_system()
{
    reset_vector_of_elements();
    elements[0] = new DC_engine;
    elements[1] = new PID_regulator;
    elements[2] = new Reference_signal_definder_static;
    elements[3] = new DC_source;
    for (auto & i : command_to_delete)
        i = true;
}

Automated_control_system::~Automated_control_system()
{
    for(int i = 0; i < AMOUNT_OF_ELEMENTS; ++i)
        if (command_to_delete[i]) delete elements[i];
}

void Automated_control_system::to_set_dt(double _dt)
{
    for (auto& i : elements) if(i != nullptr) i->to_set_dt(_dt);
}

const std::vector<const Automated_control_system_element_interface *> Automated_control_system::to_check_elements() const
{
    std::vector<const Automated_control_system_element_interface *> the_return {std::begin(elements), std::end(elements)};
    return the_return;
}

bool Automated_control_system::to_mount_the_element(Automated_control_system_element_interface * p_element)
{
    switch (p_element->to_check_the_type())
    {
    case Automated_control_system_element_interface::PROCESS:
        //if (command_to_delete[0]) delete p_process;
        p_process = p_element;
        reset_vector_of_elements(p_process);
        command_to_delete[0] = false;
        break;
    case Automated_control_system_element_interface::REGULATOR:
        //if (command_to_delete[1]) delete p_regulator;
        p_regulator = p_element;
        reset_vector_of_elements(p_regulator);
        command_to_delete[1] = false;
        break;
    case Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER:
        //if (command_to_delete[2]) delete p_definder;
        p_definder = p_element;
        reset_vector_of_elements(p_definder);
        command_to_delete[2] = false;
        break;
    case Automated_control_system_element_interface::ENERGY_SOURCE:
        //if (command_to_delete[3]) delete p_source;
        p_source = p_element;
        reset_vector_of_elements(p_source);
        command_to_delete[3] = false;
        break;
    default:
        return false;
    }
    return true;
}

bool Automated_control_system::to_mount_the_element(Automated_control_system_element_interface & r_element)
{
    return to_mount_the_element(&r_element);
}

void Automated_control_system::to_calculate() // can be paralleled
{
    if(p_regulator->to_check_the_type() == Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER)
        ((PID_regulator*)p_regulator)->to_receive_reference_signal(p_definder->to_get_output_signal());
    else
        std::cerr << "BAD CUSTOM_DYNAMIC_CAST\n";
    p_regulator->to_receive_input_signal(p_process->to_get_output_signal());

    p_source->to_receive_input_signal(p_regulator->to_get_output_signal());

    p_process->to_receive_input_signal(p_source->to_get_output_signal());

    for(auto i : elements) i->to_calculate();
}
