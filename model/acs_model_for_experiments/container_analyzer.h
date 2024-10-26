#ifndef CONTAINER_ALANYZER_H
#define CONTAINER_ALANYZER_H

#include <algorithm>
#include <functional>

template <typename type>
class container_analyzer
{
private:
    bool increasing = true;
protected:
    bool is_increasing() const { return increasing; }
public:
    container_analyzer();
    bool is_oscillating(std::vector<double>&);
    type* to_detect_extremum(type * _element_begin, type * _element_end);
};

template <typename type>
type* container_analyzer<type>::to_detect_extremum(type * _element_begin, type * _element_end)
{
    auto ans_1 = std::adjacent_find(_element_begin, _element_end, std::greater());
    auto ans_0 = std::adjacent_find(_element_begin, _element_end, std::less());
}

template <typename type>
bool container_analyzer<type>::is_oscillating(std::vector<double>& p_arr)
{
    return true;
}

#endif // CONTAINER_ALANYZER_H
