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

#include "regulator.h"

double PID_regulator::error()
{
    return parameters[REFERENCE_SIGNAL] - parameters[INPUT_SIGNAL];
}

double PID_regulator::output()
{
    return P() + I() + D();
}

double PID_regulator::P()
{
    return parameters[PROPORTIONAL_SIGNAL] = parameters[K_P] * parameters[ERROR_SIGNAL];
}

double PID_regulator::I()
{
    return parameters[INTEGRAL_SIGNAL] += parameters[K_I] * parameters[ERROR_SIGNAL] * parameters[DT];
}

double PID_regulator::D() // can be paralleled
{
    parameters[DERIVATIVE_SIGNAL] =  parameters[K_D] * ( parameters[ERROR_SIGNAL] - parameters[PREVIOUS_ERROR] ) / parameters[DT];
    parameters[PREVIOUS_ERROR] = parameters[ERROR_SIGNAL];
    return parameters[DERIVATIVE_SIGNAL];
}

PID_regulator::PID_regulator() : Automated_control_system_element_interface()
{
    the_type = PID_regulator::REGULATOR;
    while (parameters.size() < SIZE) parameters.push_back(0);
}

void PID_regulator::to_set_koefficients(double kp, double ki, double kd)
{
    parameters[K_P] = kp > 0 ? kp : 0;
    parameters[K_I] = ki > 0 ? ki : 0;
    parameters[K_D] = kd > 0 ? kd : 0;
}

void PID_regulator::to_receive_reference_signal(double r)
{
    parameters[REFERENCE_SIGNAL] = r;
}

bool PID_regulator::to_verify_amount_of_parameters() const
{
    if (parameters.size() != SIZE) return false;
    return true;
}

bool PID_regulator::to_set_element_parameters(const std::vector<double> & _r_parameters)
{
    {
        auto i = std::begin(parameters);
        std::advance(i, END_INTERFACE);
        auto j = std::begin(_r_parameters);
        std::advance(j, END_INTERFACE);
        int k = 0;
        for (
                ;
                i != std::end(parameters) && j != std::end(_r_parameters) && k < END_REGULATOR;
                ++i, ++j, ++k
             )
            *i = *j;
        if ( !(i == std::end(parameters) && j == std::end(_r_parameters)) ) return false;
    }
    return true;
}

bool PID_regulator::to_set_all_parameters(const std::vector<double> & _r_parameters)
{
    {
        auto i = std::begin(parameters);
        auto j = std::begin(_r_parameters);
        int k = 0;
        for (
                ;
                i != std::end(parameters) && j != std::end(_r_parameters) && k < END_REGULATOR;
                ++i, ++j, ++k
             )
            *i = *j;
        if ( !(i == std::end(parameters) && j == std::end(_r_parameters)) ) return false;
    }
    return true;
}

void PID_regulator::to_calculate()
{
    parameters[ERROR_SIGNAL] = error();
    parameters[OUTPUT_SIGNAL] = output();
}
