#ifndef REFERENCE_SIGNAL_DEFINDER_STATIC_H
#define REFERENCE_SIGNAL_DEFINDER_STATIC_H

#include "automated_control_system_element_interface.h"

class Reference_signal_definder_static : public Automated_control_system_element_interface
{
public:
    Reference_signal_definder_static(double=0);

    void to_set_signal(double);

    enum { SIZE = END_INTERFACE };

    //interface:
    virtual bool to_verify_amount_of_parameters() const override;
    virtual bool to_set_element_parameters(const std::vector<double> &) override;
    virtual bool to_set_all_parameters(const std::vector<double> &) override;
    virtual void to_calculate() override;
};

#endif // REFERENCE_SIGNAL_DEFINDER_STATIC_H
