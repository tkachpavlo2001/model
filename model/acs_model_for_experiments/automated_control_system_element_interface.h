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

#ifndef AUTOMATED_CONTROL_SYSTEM_ELEMENT_INTERFACE_H
#define AUTOMATED_CONTROL_SYSTEM_ELEMENT_INTERFACE_H

#include<vector>

class Automated_control_system_element_interface
{
protected:
    std::vector<double> parameters;
public:
    Automated_control_system_element_interface();
    enum type_of_element
    {
        PROCESS,
        REGULATOR,
        REFERENCE_SIGNAL_DEFINDER,
        ENERGY_SOURCE
    };
protected:
    type_of_element the_type;
public:
    const int to_check_the_type() const;
    enum
    {
        BEGIN_INTERFACE = 0,

        DT = BEGIN_INTERFACE + 0,
        T = BEGIN_INTERFACE + 1,
        INPUT_SIGNAL = BEGIN_INTERFACE + 2,
        OUTPUT_SIGNAL = BEGIN_INTERFACE + 3,


        LAST_INTERFACE = OUTPUT_SIGNAL,                         // END_INTERFACE DEPENDS ON THIS!!! DO NOT CHANGE! (WITHOUT EXAMINATION)
        END_INTERFACE = LAST_INTERFACE + 1                      // DEPENDANT ON LAST_INTERFACE
    };
    bool to_set_dt(double const &);
    bool to_receive_input_signal(double const &);
    double to_get_output_signal();
    bool to_set_configurative_parameters(const std::vector<double> &);
    const std::vector<double> & to_check_parameters() const;
    std::vector<double> & to_get_parameters();
    unsigned int to_check_amount_of_parameters() const;

    virtual ~Automated_control_system_element_interface() = 0;

    //interface:
    virtual bool to_verify_amount_of_parameters() const = 0;
    virtual bool to_set_element_parameters(const std::vector<double> &) = 0;
    virtual bool to_set_all_parameters(const std::vector<double> &) = 0;
    virtual void to_calculate() = 0;                                                // missed
};

#endif // AUTOMATED_CONTROL_SYSTEM_ELEMENT_INTERFACE_H
