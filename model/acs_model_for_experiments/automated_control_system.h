#ifndef AUTOMATED_CONTROL_SYSTEM_H
#define AUTOMATED_CONTROL_SYSTEM_H

#include"controlled_process.h"
#include"regulator.h"
#include"reference_signal_definder_static.h"
#include"dc_source.h"
#include<vector>
#include<array>

class Automated_control_system
{
private:
    DC_engine * p_process;
    PID_regulator * p_regulator;
    Reference_signal_definder_static * p_definder;
    DC_source * p_source;
    double dt;
    double t;

    void reset_vector_of_elements(Automated_control_system_element_interface * = nullptr);
    Automated_control_system_element_interface * to_get_certain_element(Automated_control_system_element_interface::type_of_element);
protected:
    std::vector<Automated_control_system_element_interface *> elements;
    void to_communicate();
    void to_plus_dt();
public:
    Automated_control_system();
    ~Automated_control_system();
    void to_set_dt(double);
    double to_check_dt() const; //not tested
    void to_set_t(double = 0); //not tested
    double to_check_t() const; //not tested
    bool to_mount_the_element(Automated_control_system_element_interface *);
    bool to_mount_the_element(Automated_control_system_element_interface &);
    const std::vector<const Automated_control_system_element_interface *> to_check_elements() const;
    const std::vector<const Automated_control_system_element_interface *> to_check_ordered_elements() const;
    const Automated_control_system_element_interface * to_check_certain_element(Automated_control_system_element_interface::type_of_element) const;
    const Automated_control_system_element_interface * to_check_process() const;
    const Automated_control_system_element_interface * to_check_regulator() const;
    const Automated_control_system_element_interface * to_check_definder() const;
    const Automated_control_system_element_interface * to_check_source() const;
    virtual void to_calculate();
};

class Automated_control_system_paralleled : public Automated_control_system
{
    virtual void to_calculate() override;
};

#endif // AUTOMATED_CONTROL_SYSTEM_H
