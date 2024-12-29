/*
 * This file is part of ACS_MODEL.
 *
 * ACS_MODEL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACS_MODEL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACS_MODEL. If not, see <https://www.gnu.org/licenses/>.
 */

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
    DC_engine * p_process = nullptr;
    PID_regulator * p_regulator = nullptr;
    Reference_signal_definder_static * p_definder = nullptr;
    DC_source * p_source = nullptr;
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
    Automated_control_system_element_interface * to_get_definder();
    virtual void to_calculate();
};

class Automated_control_system_paralleled : public Automated_control_system
{
    virtual void to_calculate() override;
};

#endif // AUTOMATED_CONTROL_SYSTEM_H
