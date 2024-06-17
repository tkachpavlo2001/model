#ifndef CONTROLLED_PROCESS_H
#define CONTROLLED_PROCESS_H

#include "automated_control_system_element_interface.h"

class DC_engine : public Automated_control_system_element_interface
{
private:
    enum
    {
        LAST_DC_ENGINE = 0
    };
    bool to_check_amount_of_parameters() const override;
protected:
    enum
    {
        END_DC_ENGINE = (LAST_DC_ENGINE + 1),
        SIZE = END_DC_ENGINE
    };
public:
    DC_engine();
    virtual ~DC_engine() override;
    bool to_set_element_parameters(const std::vector<double> &) override;
    bool to_set_all_parameters(const std::vector<double> &) override;
    void to_calculate() override;
};

#endif // CONTROLLED_PROCESS_H
