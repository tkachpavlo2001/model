#ifndef REGULATOR_H
#define REGULATOR_H

#include"automated_control_system_element_interface.h"

class PID_regulator : public Automated_control_system_element_interface
{
public:
    PID_regulator();
};

#endif // REGULATOR_H
