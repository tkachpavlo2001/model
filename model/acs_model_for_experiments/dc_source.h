#ifndef DC_SOURCE_H
#define DC_SOURCE_H

#include "automated_control_system_element_interface.h"

class DC_source : public Automated_control_system_element_interface
{
public:
    DC_source();
    enum
    {
        BEGIN_SOURCE = END_INTERFACE,           /// BE CRARE FOR THE TRUE OF THE EQUATION !!

        MAX_VOLTAGE = BEGIN_SOURCE + 0,

        MIN_VOLTAGE = BEGIN_SOURCE + 1,

        LAST_SOURCE = MIN_VOLTAGE,              /// BE CRARE FOR THE TRUE OF THE EQUATION !!
        END_SOURCE = LAST_SOURCE + 1,
        SIZE = END_SOURCE
    };
    void to_set_max_voltage(double);
    void to_set_min_voltage(double);
    //interface:
    virtual bool to_verify_amount_of_parameters() const override;
    virtual bool to_set_element_parameters(const std::vector<double> &) override;
    virtual bool to_set_all_parameters(const std::vector<double> &) override;
    virtual void to_calculate() override;
};

class DC_source_inerted : public DC_source
{
private:
    double change_step();
public:
    virtual void to_calculate() override;
};

#endif // DC_SOURCE_H
