#ifndef TKACHPAVLO2001LIB_MYRAND_H
#define TKACHPAVLO2001LIB_MYRAND_H

#include<random>
inline double myrand() { return double(rand() % 101) / 100; }

#endif // !TKACHPAVLO2001LIB_MYRAND_H
