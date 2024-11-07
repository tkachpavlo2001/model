#include "automated_control_system.h"

#include<iostream>


void Automated_control_system::reset_vector_of_elements(Automated_control_system_element_interface * p_new_element)
{
    if(p_new_element != nullptr)
    {
        bool done = false;
        int in_type = p_new_element->to_check_the_type();
        for (auto & i : elements)
        {
            if(i == nullptr)
                continue;
            else
            {
                if(in_type == Automated_control_system_element_interface::PROCESS && dynamic_cast<DC_engine *>(i) != nullptr)
                {
                    i = p_new_element;
                    if (done) std::cerr << "reset_vector_of_elements FAULT 1";
                    done = true;
                }
                if(in_type == Automated_control_system_element_interface::REGULATOR && dynamic_cast<PID_regulator *>(i) != nullptr)
                {
                    i = p_new_element;
                    if (done) std::cerr << "reset_vector_of_elements FAULT 1";
                    done = true;
                }
                if(in_type == Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER && dynamic_cast<Reference_signal_definder_static *>(i) != nullptr)
                {
                    i = p_new_element;
                    if (done) std::cerr << "reset_vector_of_elements FAULT 1";
                    done = true;
                }
                if(in_type == Automated_control_system_element_interface::ENERGY_SOURCE && dynamic_cast<DC_source *>(i) != nullptr)
                {
                    i = p_new_element;
                    if (done) std::cerr << "reset_vector_of_elements FAULT 1";
                    done = true;
                }
            }
        }
        if(!done)
            for (auto & i : elements)
                if(i == nullptr)
                {
                    i = p_new_element;
                    done = true;
                    break;
                }
        if(!done)
            std::cerr << "reset_vector_of_elements FAULT 2";
    }
    else
    {
        elements.push_back(p_process);
        elements.push_back(p_regulator);
        elements.push_back(p_definder);
        elements.push_back(p_source);
    }
}

Automated_control_system_element_interface * Automated_control_system::to_get_certain_element(Automated_control_system_element_interface::type_of_element TYPE)
{
    for (auto & i : elements)
        if (i != nullptr)
            if (i->to_check_the_type() == TYPE) return i;
    return nullptr;
}

Automated_control_system::Automated_control_system()
{
    reset_vector_of_elements();
    for (auto & i : elements)
        i = nullptr;
    t = 0;
    dt = 0;
}

Automated_control_system::~Automated_control_system()
{
}

void Automated_control_system::to_set_dt(double _dt)
{
    dt = _dt;
    for (auto& i : elements) if(i != nullptr) i->to_set_dt(_dt);
}

double Automated_control_system::to_check_dt() const //not tested
{
    return dt;
}

void Automated_control_system::to_set_t(double _t) //not tested
{
    t = _t;
}

double Automated_control_system::to_check_t() const //not tested
{
    return t;
}

const std::vector<const Automated_control_system_element_interface *> Automated_control_system::to_check_elements() const
{
    std::vector<const Automated_control_system_element_interface *> the_return {std::begin(elements), std::end(elements)};
    return the_return;
}

const std::vector<const Automated_control_system_element_interface *> Automated_control_system::to_check_ordered_elements() const
{
    std::vector<const Automated_control_system_element_interface *> the_return;
    the_return.push_back(to_check_definder());
    the_return.push_back(to_check_regulator());
    the_return.push_back(to_check_source());
    the_return.push_back(to_check_process());
    return the_return;
}

bool Automated_control_system::to_mount_the_element(Automated_control_system_element_interface * p_element)
{
    switch (p_element->to_check_the_type())
    {
    case Automated_control_system_element_interface::PROCESS:
        p_process = static_cast<DC_engine*>(p_element);
        reset_vector_of_elements(p_process);
        break;
    case Automated_control_system_element_interface::REGULATOR:
        p_regulator = static_cast<PID_regulator*>(p_element);
        reset_vector_of_elements(p_regulator);
        break;
    case Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER:
        p_definder = static_cast<Reference_signal_definder_static*>(p_element);
        reset_vector_of_elements(p_definder);
        break;
    case Automated_control_system_element_interface::ENERGY_SOURCE:
        p_source = static_cast<DC_source*>(p_element);
        reset_vector_of_elements(p_source);
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

void Automated_control_system::to_communicate()
{
    if ( to_check_regulator()!=nullptr && to_check_definder()!=nullptr) p_regulator->to_receive_reference_signal(p_definder->to_get_output_signal());
    if ( to_check_regulator()!=nullptr && to_check_process()!=nullptr)  p_regulator->to_receive_input_signal(p_process->to_get_output_signal());

    if ( to_check_source()!=nullptr && to_check_regulator()!=nullptr) p_source->to_receive_input_signal(p_regulator->to_get_output_signal());

    if ( to_check_process()!=nullptr && to_check_source()!=nullptr) p_process->to_receive_input_signal(p_source->to_get_output_signal());
}

void Automated_control_system::to_plus_dt()
{
    t += dt;
}

void Automated_control_system::to_calculate() // can be paralleled
{
    to_communicate();

    for(auto i : elements) if(i != nullptr) i->to_calculate();

    to_plus_dt();
}

const Automated_control_system_element_interface * Automated_control_system::to_check_certain_element(Automated_control_system_element_interface::type_of_element TYPE) const
{
    for (auto & i : elements)
        if (i != nullptr)
            if (i->to_check_the_type() == TYPE) return i;
    return nullptr;
}

DC_engine * Automated_control_system::to_check_process() const
{
    return p_process;
}

const Automated_control_system_element_interface * Automated_control_system::to_check_regulator() const
{
    return to_check_certain_element(Automated_control_system_element_interface::REGULATOR);
}

const Automated_control_system_element_interface * Automated_control_system::to_check_definder() const
{
    return to_check_certain_element(Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER);
}

const Automated_control_system_element_interface * Automated_control_system::to_check_source() const
{
    return to_check_certain_element(Automated_control_system_element_interface::ENERGY_SOURCE);
}

Automated_control_system_element_interface * Automated_control_system::to_get_definder()
{
    return p_definder;
}

#include<algorithm>
#include<execution>

void Automated_control_system_paralleled::to_calculate()
{
    to_communicate();

    std::for_each(std::execution::par,elements.begin(), elements.end(), [](auto i) {if(i != nullptr) i->to_calculate(); } );

    to_plus_dt();
}
