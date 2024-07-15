#ifndef REGULATOR_H
#define REGULATOR_H

#include"automated_control_system_element_interface.h"
#include<memory>

class PID_regulator : public Automated_control_system_element_interface
{
private:
    double error();
    double output();
    double P();
    double I();
    double D();
public:
    PID_regulator();

    void to_set_koefficients(double, double=0, double=0);
    void to_receive_reference_signal(double);

    enum {
        BEGIN_REGULATOR = END_INTERFACE,                    /// BE CRARE FOR THE TRUE OF THE EQUATION !!

        K_P = BEGIN_REGULATOR + 0,

        K_D = BEGIN_REGULATOR + 1,

        K_I = BEGIN_REGULATOR + 2,

        PROPORTIONAL_SIGNAL = BEGIN_REGULATOR + 3,

        INTEGRAL_SIGNAL = BEGIN_REGULATOR + 4,

        DERIVATIVE_SIGNAL = BEGIN_REGULATOR + 5,

        PREVIOUS_ERROR = BEGIN_REGULATOR + 6,

        REFERENCE_SIGNAL = BEGIN_REGULATOR + 7,

        ERROR_SIGNAL = BEGIN_REGULATOR + 8,

        LAST_REGULATOR = ERROR_SIGNAL,                       /// BE CRARE FOR THE TRUE OF THE EQUATION !!
        END_REGULATOR = LAST_REGULATOR + 1,
        SIZE = END_REGULATOR
    };

    //interface:
    virtual bool to_verify_amount_of_parameters() const override;
    virtual bool to_set_element_parameters(const std::vector<double> &) override;
    virtual bool to_set_all_parameters(const std::vector<double> &) override;
    virtual void to_calculate() override;
};


#endif // REGULATOR_H
