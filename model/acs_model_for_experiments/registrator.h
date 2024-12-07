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
class Registrator_qt : public Registrator, QWidget
{
private:
    QSplineSeries * _pSeries = nullptr;
    void to_record() override;
protected:
    Registrator_qt(QWidget*p) : QWidget(p), Registrator() {}
public:
    void to_set_series(QSplineSeries*arg) { _pSeries = arg; }
    static Registrator_qt * to_new(QWidget*p) { return new Registrator_qt(p); }
};

#endif // REGISTRATOR_H
