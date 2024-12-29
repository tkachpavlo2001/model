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
