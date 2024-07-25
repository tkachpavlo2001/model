#ifndef REGISTRATOR_H
#define REGISTRATOR_H

#include"automated_control_system.h"
#include<string>
#include <fstream>

class Registrator
{
private:
    const Automated_control_system * acs_model_status;
    std::string name_of_file;
    bool first_record_commited;
    virtual void to_record() = 0;
protected:
    bool to_actulize_the_fist_record_commited_status();
    const Automated_control_system * to_check_acs_model_status() const;
public:
    Registrator();
    virtual ~Registrator() = 0;
    const char * to_check_name_of_file() const;
    void to_set_name_of_file(std::string);
    Registrator & operator<<(const Automated_control_system&);
};

class Registrator_to_txt_file : public Registrator
{
private:
    std::fstream fout;
    virtual void to_record() override;
protected:
    bool to_actulize_the_fist_record_commited_status();
public:
    Registrator_to_txt_file();
    virtual ~Registrator_to_txt_file() override;
};

#endif // REGISTRATOR_H
