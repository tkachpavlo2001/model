#include "registrator.h"

bool Registrator::to_actulize_the_fist_record_commited_status()
{
    if(!first_record_commited)
    {
        first_record_commited = true;
        return false;
    }
    return true;
}

const Automated_control_system * Registrator::to_check_acs_model_status() const
{
    return acs_model_status;
}

Registrator::Registrator() : acs_model_status(nullptr), name_of_file("the_last_model's_run_records"), first_record_commited(false)
{}

Registrator::~Registrator()
{}

const char * Registrator::to_check_name_of_file() const
{
    return name_of_file.c_str();
}

void Registrator::to_set_name_of_file(std::string _new_name)
{
    name_of_file = _new_name;
}

Registrator & Registrator::operator<<(const Automated_control_system& acs_model)
{
    acs_model_status = &acs_model;
    to_record();
    return *this;
}

Registrator_to_txt_file::Registrator_to_txt_file() : Registrator()
{}

Registrator_to_txt_file::~Registrator_to_txt_file()
{
    fout.close();
}


bool Registrator_to_txt_file::is_open()
{
    return fout.is_open();
}

void Registrator_to_txt_file::to_record()
{
    if(!Registrator::to_actulize_the_fist_record_commited_status())
    {
        std::string file_txt_name = std::string(to_check_name_of_file()) + std::string(".txt");
        fout.open(file_txt_name);
    }

    const Automated_control_system & acs_model = *to_check_acs_model_status();

    if(to_check_acs_model_status() != nullptr) fout << acs_model.to_check_t() << '\t';
    if(to_check_acs_model_status() != nullptr) for (auto & i : acs_model.to_check_ordered_elements())
        if (i != nullptr) for (auto & j : i->to_check_parameters())
            fout << j << '\t';
    fout << std::endl;
}

void Registrator_to_txt_file_short::to_record()
{
    if(!Registrator::to_actulize_the_fist_record_commited_status())
    {
        std::string file_txt_name = std::string(to_check_name_of_file()) + std::string(".txt");
        fout.open(file_txt_name);
    }

    const Automated_control_system & acs_model = *to_check_acs_model_status();

    if(to_check_acs_model_status() != nullptr) fout << acs_model.to_check_t() << '\t';
    if(to_check_acs_model_status() != nullptr) for (auto & i : acs_model.to_check_ordered_elements())
        if (i != nullptr) for (auto & j : i->to_check_parameters())
        {
            if (
                i->to_check_the_type() == Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER && &j == &(i->to_check_parameters()[Reference_signal_definder_static::OUTPUT_SIGNAL])
                    ||
                i->to_check_the_type() == Automated_control_system_element_interface::PROCESS && &j == &(i->to_check_parameters()[DC_engine::VELOCITY])
                    ||
                i->to_check_the_type() == Automated_control_system_element_interface::PROCESS && &j == &(i->to_check_parameters()[DC_engine::TORQUE_OF_LOAD])
                    ||
                i->to_check_the_type() == Automated_control_system_element_interface::PROCESS && &j == &(i->to_check_parameters()[DC_engine::VOLTAGE])

                )
            fout << j << '\t';
        }
    fout << std::endl;
}
Registrator_to_txt_file_short::Registrator_to_txt_file_short() : Registrator_to_txt_file()
{
    ;
}
Registrator_to_txt_file_short::~Registrator_to_txt_file_short()
{
    ;
}
