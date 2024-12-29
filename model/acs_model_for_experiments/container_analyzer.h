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

#ifndef CONTAINER_ALANYZER_H
#define CONTAINER_ALANYZER_H

#include <algorithm>
#include <functional>
#include <numeric>

class container_analyzer
{
private:
    bool increasing = true;
protected:
    bool is_increasing() const { return increasing; }
    void to_change_increasing_status() { increasing = !increasing; }
    std::vector<double>::iterator to_detect_extremum_Get_p(const std::vector<double>::iterator&, const std::vector<double>::iterator&);
    bool is_stable_the_oscilations(const std::vector<int>&);
    bool is_stable_the_amplitude(const std::vector<double>&);
    std::vector<int> to_calculate_periods_Get(const std::vector<int>&);
    std::vector<double> to_calculate_amplitudes_Get(const std::vector<double>&);
    void to_calculate_extremums_and_seconds_Put_in(std::vector<double>& r_vector_obj, std::vector<double>& extremums,std::vector<int>& seconds);
public:
    container_analyzer() {}
    bool is_oscillating(std::vector<double>&);
    double to_calculate_period_in_Get(std::vector<double>&,double);
};


#endif // CONTAINER_ALANYZER_H
