#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

static const int a = 0;

class Chart_abstract_factory
{
public:
    Chart_abstract_factory(){}
};

class Chart_config_abstract_factory
{
public:
    Chart_config_abstract_factory(){}
};

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT
private:
    QPushButton * _pbutton_velocity_run;
    QPushButton * _pbutton_theta_run;
    QPushButton * _pregulator_run;
    enum mode
    {
        VELOCITY,
        THETA,
        REGULATOR
    };
    mode _ebutton_chosed = VELOCITY;
    Chart_abstract_factory _aChart_abstract_factory;
    Chart_config_abstract_factory _aChart_config_abstract_factory;
    virtual QChartView * _to_new_abstract_chart();
    virtual QWidget * _to_new_abstract_chart_config();
protected:
    MainWindow()
    {
    }
    ~MainWindow() override;
private slots:
    QChartView * to_show_chart();
    QWidget * to_show_chart_config();
public:
    static MainWindow * to_new()
    {
        return new MainWindow();
    }
};


#endif // MAINWINDOW_H
