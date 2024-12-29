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

#ifndef REGISTRATOR_H
#define REGISTRATOR_H

#include"automated_control_system.h"
#include<string>
#include <fstream>
#include <vector>

#include <QDebug>

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
    Registrator();
public:
    virtual ~Registrator() = 0;

    const char * to_check_name_of_file() const;
    void to_set_name_of_file(std::string);
    Registrator & operator<<(const Automated_control_system&);
};

class Registrator_to_txt_file : public Registrator
{
private:
    virtual void to_record() override;
protected:
    std::ofstream fout;
    bool to_actulize_the_fist_record_commited_status();
public:
    Registrator_to_txt_file();
    virtual ~Registrator_to_txt_file() override;
    bool is_open();
};

class Registrator_to_txt_file_short : public Registrator_to_txt_file
{
private:
    virtual void to_record() override;
public:
    Registrator_to_txt_file_short();
    virtual ~Registrator_to_txt_file_short() override;
};

class Registrator_to_std_vector : public Registrator
{
public:
    void to_record() override;
protected:
    std::vector<double> * records = nullptr;
public:
    Registrator_to_std_vector();
    virtual ~Registrator_to_std_vector();
    void to_set_vector(std::vector<double>&);
    void to_set_vector(std::vector<double>*);
    void to_set_vector(std::shared_ptr<std::vector<double>>);
};

class Registrator_to_std_vector_difference : public Registrator_to_std_vector
{
    void to_record() override;
};

#include <QApplication>
#include <QtCharts/QSplineSeries>
#include <QWidget>
class iChartWidget;
class Registrator_qt : public Registrator, QWidget
{
private:
    double _dt_to_plot = 0;
    QXYSeries * _pSeries = nullptr;
    QXYSeries * _pSeries_additional = nullptr;
    iChartWidget * _pChart = nullptr;
    void to_record() override;
protected:
    Registrator_qt(QWidget*p) : QWidget(p), Registrator() {}
    void _to_update_chart();
public:
    void to_set_series(QXYSeries*arg) { _pSeries = arg; }
    void to_set_chart(iChartWidget*arg) { _pChart = arg; }
    void to_set_series_additional(QXYSeries*arg) { _pSeries_additional = arg; }
    void to_set_dt_to_plot(double n) { _dt_to_plot = n; }
    static Registrator_qt * to_new(QWidget*p) { return new Registrator_qt(p); }
};

#endif // REGISTRATOR_H
