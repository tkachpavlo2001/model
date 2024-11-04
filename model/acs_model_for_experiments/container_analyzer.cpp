#include"container_analyzer.h"

std::vector<double>::iterator container_analyzer::to_detect_extremum_Get_p(const std::vector<double>::iterator& it_begin, const std::vector<double>::iterator& it_end)
{
    //data
    auto ans = it_begin;
    //alg
    if (is_increasing())
    {
        ans = std::adjacent_find(it_begin, it_end, std::greater());
    }
    else
    {
        ans = std::adjacent_find(it_begin, it_end, std::less());
    }

    if (ans != it_end) to_change_increasing_status();

    return ans;
}

bool container_analyzer::is_oscillating(std::vector<double>& r_vector_obj)
{
    //data
    std::vector<double> extremums;
    std::vector<int> seconds;
    bool isStable_last_check = false;
    //alg
    to_calculate_extremums_and_seconds_Put_in(r_vector_obj, extremums, seconds);
    isStable_last_check = is_stable_the_oscilations(seconds);
    isStable_last_check *= is_stable_the_amplitude(extremums);

    return isStable_last_check;
}

void container_analyzer::to_calculate_extremums_and_seconds_Put_in(std::vector<double>& r_vector_obj, std::vector<double>& extremums,std::vector<int>& seconds)
{
    //data
    std::vector<double>::iterator p_extremum;
    std::vector<double>::iterator p_extremum_prev;
    //alg
    p_extremum = to_detect_extremum_Get_p(r_vector_obj.begin(), r_vector_obj.end());
    p_extremum_prev = p_extremum;
    while( p_extremum != r_vector_obj.end() )
    {
        extremums.push_back(*p_extremum);
        if ( p_extremum != p_extremum_prev ) seconds.push_back(p_extremum - p_extremum_prev);
        if ( p_extremum != p_extremum_prev ) p_extremum_prev = p_extremum;

        p_extremum = to_detect_extremum_Get_p(p_extremum, r_vector_obj.end());
    }
}

bool container_analyzer::is_stable_the_oscilations(const std::vector<int>& seconds)
{
    //data
    std::vector<int> amplitudes;
    double average_amplitude = 0;
    bool ans = true;
    //alg
    amplitudes = to_calculate_periods_Get(seconds);
    average_amplitude = double( std::accumulate(amplitudes.begin(), amplitudes.end(), 0) ) / double(amplitudes.size());
    for (auto i : amplitudes)
        ans *= (average_amplitude * 0.9 < i && i < average_amplitude * 1.1) ;
    return ans;
}

std::vector<int> container_analyzer::to_calculate_periods_Get(const std::vector<int>& seconds)
{
    //data
    std::vector<int> periods;
    int period = 0;
    //alg
    auto i_prev = seconds.begin();
    for(auto i = seconds.begin(); i < seconds.end(); ++i)
    {
        period += *i;
        if (i - i_prev != 0)
        {
            periods.push_back(period);
            i_prev = i + 1;
            period = 0;
        }
    }
    return periods;
}

bool container_analyzer::is_stable_the_amplitude(const std::vector<double>& extremums)
{
    //data
    std::vector<double> amplitudes;
    double average_amplitude;
    bool ans = true;
    //alg
    amplitudes = to_calculate_amplitudes_Get(extremums);
    average_amplitude = std::accumulate(amplitudes.begin(), amplitudes.end(), 0) / double(amplitudes.size());
    for (auto i : amplitudes)
        ans *= (average_amplitude * 0.9 < i && i < average_amplitude * 1.1) ;
    if (amplitudes.size() < 2) ans = false;
    return ans;
}

std::vector<double> container_analyzer::to_calculate_amplitudes_Get(const std::vector<double>& extremums)
{
    //data
    std::vector<double> amplitudes;
    //alg
    auto i_prev = extremums.begin();
    for(auto i = extremums.begin() + 1; i < extremums.end(); ++i)
    {
        amplitudes.push_back( std::abs(*i - *i_prev) / 2 );
        i_prev = i;
    }
    return amplitudes;
}

double container_analyzer::to_calculate_period_in_Get(std::vector<double>& r_vector_obj, double dt)
{
    //data
    std::vector<double> extremums;
    std::vector<int> seconds;
    std::vector<int> periods;
    double average_period;
    //alg
    to_calculate_extremums_and_seconds_Put_in(r_vector_obj, extremums, seconds);
    periods = to_calculate_periods_Get(seconds);
    average_period = double( std::accumulate(periods.begin(), periods.end(), 0) ) / double(periods.size());
    return average_period * dt;
}
