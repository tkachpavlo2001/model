#ifndef AUTOMATED_CONTROL_SYSTEM_H
#define AUTOMATED_CONTROL_SYSTEM_H

#include"controlled_process.h"
#include"regulator.h"
#include"reference_signal_definder_static.h"
#include<vector>

class Automated_control_system
{
private:
    Automated_control_system_element_interface * p_process;
    Automated_control_system_element_interface * p_regulator;
    Automated_control_system_element_interface * p_definder;
    std::vector<Automated_control_system_element_interface *> elements;
public:
    Automated_control_system();
    ~Automated_control_system();
    void to_set_dt(double);
    bool to_mount_the_element(Automated_control_system_element_interface *);
    bool to_mount_the_element(Automated_control_system_element_interface &);
    void to_calculate();
};

#endif // AUTOMATED_CONTROL_SYSTEM_H
