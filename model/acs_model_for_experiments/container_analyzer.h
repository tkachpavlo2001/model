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
