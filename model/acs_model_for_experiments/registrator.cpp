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

#include "registrator.h"
#include "iostream"

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

        fout << "t\tr\tPID\tSource\tVelocity\tEngVoltage\tTorque\n";
        std::cout << "t\tr\tPID\tSource\tVelocity\tEngVoltage\tTorque\n";
    }

    const Automated_control_system & acs_model = *to_check_acs_model_status();

    if(to_check_acs_model_status() != nullptr)
    {
        fout << acs_model.to_check_t() << '\t';
        std::cout << acs_model.to_check_t() << '\t';
    }
    if(to_check_acs_model_status() != nullptr) for (auto & i : acs_model.to_check_ordered_elements())
        if (i != nullptr) for (auto & j : i->to_check_parameters())
        {
            if (
                i->to_check_the_type() == Automated_control_system_element_interface::REFERENCE_SIGNAL_DEFINDER && &j == &(i->to_check_parameters()[Reference_signal_definder_static::OUTPUT_SIGNAL])
                    ||
                i->to_check_the_type() == Automated_control_system_element_interface::REGULATOR && &j == &(i->to_check_parameters()[PID_regulator::OUTPUT_SIGNAL])
                    ||
                i->to_check_the_type() == Automated_control_system_element_interface::ENERGY_SOURCE && &j == &(i->to_check_parameters()[DC_source::OUTPUT_SIGNAL])
                    ||
                i->to_check_the_type() == Automated_control_system_element_interface::PROCESS && &j == &(i->to_check_parameters()[DC_engine::OUTPUT_SIGNAL])
                    ||
                i->to_check_the_type() == Automated_control_system_element_interface::PROCESS && &j == &(i->to_check_parameters()[DC_engine::TORQUE_OF_LOAD])
                    ||
                i->to_check_the_type() == Automated_control_system_element_interface::PROCESS && &j == &(i->to_check_parameters()[DC_engine::VOLTAGE])
                )
            {
                fout << j << '\t';
                std::cout << j << '\t';
            }
        }
    fout << std::endl;
    std::cout << std::endl;
}
Registrator_to_txt_file_short::Registrator_to_txt_file_short() : Registrator_to_txt_file()
{
    ;
}
Registrator_to_txt_file_short::~Registrator_to_txt_file_short()
{
    ;
}



void Registrator_to_std_vector::to_record()
{
    if (records != nullptr) records->push_back(to_check_acs_model_status()->to_check_process()->to_check_parameters()[DC_engine::OUTPUT_SIGNAL]);
}
Registrator_to_std_vector::Registrator_to_std_vector()
{}
Registrator_to_std_vector::~Registrator_to_std_vector()
{

}
void Registrator_to_std_vector::to_set_vector(std::vector<double> & _vector)
{
    to_set_vector(&_vector);
}
void Registrator_to_std_vector::to_set_vector(std::vector<double>* _vector)
{
    records = _vector;
}
void Registrator_to_std_vector::to_set_vector(std::shared_ptr<std::vector<double>> _vector)
{
    to_set_vector(_vector.get());
}


void Registrator_to_std_vector_difference::to_record()
{
    bool COEFITIENT_LIMITING_MODE = false;
    double a = to_check_acs_model_status()->to_check_regulator()->to_check_parameters()[PID_regulator::OUTPUT_SIGNAL] ;
    double b = to_check_acs_model_status()->to_check_source()->to_check_parameters()[DC_source::MAX_VOLTAGE];
    double c = to_check_acs_model_status()->to_check_process()->to_check_parameters()[DC_engine::OUTPUT_SIGNAL];
    double d = to_check_acs_model_status()->to_check_definder()->to_check_parameters()[Automated_control_system_element_interface::OUTPUT_SIGNAL];
    if (records == nullptr) { std::cerr << "to_record() error"; return; }
    if (COEFITIENT_LIMITING_MODE)
    {
        records->push_back(
                (a>b) ?
                (a/b) : (1)
                *
                (d-c)
            );
    }
    else records->push_back(d-c);

}

void Registrator_qt::to_record()
{
    if(_pSeries==nullptr) { qDebug() << "Registrator error"; abort(); }
    double dt = to_check_acs_model_status()->to_check_process()->to_check_parameters()[DC_engine::DT] ;
    double out = to_check_acs_model_status()->to_check_process()->to_check_parameters()[DC_engine::OUTPUT_SIGNAL];
    static double out_additional = 0;
    if ( to_check_acs_model_status()->to_check_definder() != nullptr ) out_additional = to_check_acs_model_status()->to_check_definder()->to_check_parameters()[Reference_signal_definder_static::OUTPUT_SIGNAL];
    else out_additional = to_check_acs_model_status()->to_check_source()->to_check_parameters()[DC_source::OUTPUT_SIGNAL];
    double x;

    if(_pSeries->count() != 0) x = _pSeries->at(_pSeries->count() - 1).x() + _dt_to_plot;
    else x = 0;

    _pSeries->append( x, out );
    _pSeries_additional->append( x, out_additional );

    _to_update_chart();
}

#include"chartwidgetfactory.h"
#include<QDebug>
void Registrator_qt::_to_update_chart()
{
    if ( this->parent() != nullptr ) (static_cast<iChartWidget*>(this->parent()))->to_update_chart();
    else qDebug() << "&Registrator_qt::_to_update_chart ERROR";
}
