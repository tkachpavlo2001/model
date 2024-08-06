#ifndef REGULATOR_TUNER_H
#define REGULATOR_TUNER_H


#include"automated_control_system.h"

#include <memory>

class Regulator_tuner_iterface
{
private:
    std::shared_ptr<Automated_control_system> acs_model;
    std::shared_ptr<PID_regulator> controller;
public:
    Regulator_tuner_iterface();
    virtual ~Regulator_tuner_iterface() = 0;
    void to_set_model_and_regulator(std::shared_ptr<Automated_control_system>, std::shared_ptr<PID_regulator>);

};

class Regulator_tuner : public Regulator_tuner_iterface
{
private:
public:
    Regulator_tuner();
    virtual ~Regulator_tuner();
};

#endif // REGULATOR_TUNER_H
