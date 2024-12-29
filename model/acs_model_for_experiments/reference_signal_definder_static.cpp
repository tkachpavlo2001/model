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

#include "reference_signal_definder_static.h"

Reference_signal_definder_static::Reference_signal_definder_static(double r) : Automated_control_system_element_interface()
{
    the_type = Reference_signal_definder_static::REFERENCE_SIGNAL_DEFINDER;
    while (parameters.size() < SIZE) parameters.push_back(0);
    to_set_signal(r);
}

void Reference_signal_definder_static::to_set_signal(double r)
{
    parameters[OUTPUT_SIGNAL] = r;
}

bool Reference_signal_definder_static::to_verify_amount_of_parameters() const
{
    if (parameters.size() != SIZE) return false;
    return true;
}

bool Reference_signal_definder_static::to_set_element_parameters(const std::vector<double> & _r_parameters)
{
    return false;
}

bool Reference_signal_definder_static::to_set_all_parameters(const std::vector<double> & _r_parameters)
{
    {
        auto i = std::begin(parameters);
        auto j = std::begin(_r_parameters);
        int k = 0;
        for (
                ;
                i != std::end(parameters) && j != std::end(_r_parameters) && k < SIZE;
                ++i, ++j, ++k
             )
            *i = *j;
        if ( !(i == std::end(parameters) && j == std::end(_r_parameters)) ) return false;
    }
    return true;
}

void Reference_signal_definder_static::to_calculate()
{}
