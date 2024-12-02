#ifndef CHARTWIDGETFACTORY_H
#define CHARTWIDGETFACTORY_H

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QLCDNumber>
#include <QMessageBox>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

class ChartWidget : public QWidget
{
private:
    Q_OBJECT
private:
    QChartView * _pMainChart = nullptr;
    QChart * _pBackChart = nullptr;
    QSplineSeries * _pSeries = nullptr;
    QSlider * _pInputSlider = nullptr;
    QSlider * _pk0Slider = nullptr;
    QSlider * _pk1Slider = nullptr;
    QLCDNumber * _pOutputNumber = nullptr;
    void _to_init()
    {
        ;
    }
    void _to_init_0()
    {
        _to_init();
        ;
    }
    void _to_init_1()
    {
        _to_init();
        ;
    }
    void _to_init_2()
    {
        _to_init();
        ;
    }
protected:
    ChartWidget(QWidget*p_parrent, int=0);
public:
    static ChartWidget * to_new_velocity(QWidget*p_parrent) { return new ChartWidget(p_parrent); }
    static ChartWidget * to_new_theta(QWidget*p_parrent) { return new ChartWidget(p_parrent, 1); }
    static ChartWidget * to_new_regulator(QWidget*p_parrent) { return new ChartWidget(p_parrent, 2); }
};

class ChartWidgetConfig : public QWidget
{
private:
    Q_OBJECT
private:
    QBoxLayout * _pmainLayout = nullptr;
    QLabel * _pInputLabel = nullptr;
    QLineEdit * _pInputEdit = nullptr;
    QLineEdit * _pk0Edit = nullptr;
    QLineEdit * _pk1Edit = nullptr;
    QPushButton * _pRunButton = nullptr;
    QPushButton * _pApplyButton = nullptr;
    QLineEdit * _pkpEdit = nullptr;
    QLineEdit * _pkiEdit = nullptr;
    QLineEdit * _pkdEdit = nullptr;
    void _to_init()
    {
        _pmainLayout = new QVBoxLayout;

        QHBoxLayout * p_primeInputLayout = new QHBoxLayout;
        _pmainLayout->addLayout(p_primeInputLayout);

        _pInputLabel = new QLabel;
        _pInputLabel->setText("NODATA");
        p_primeInputLayout->addWidget(_pInputLabel);

        _pInputEdit = new QLineEdit;
        p_primeInputLayout->addWidget(_pInputEdit);

        // ...
    }
    void _to_init_0()
    {
        _to_init();
        ;
    }
    void _to_init_1()
    {
        _to_init();
        ;
    }
    void _to_init_2()
    {
        _to_init();
        QHBoxLayout * p_regulatorInputLayout = new QHBoxLayout;
        _pmainLayout->insertLayout(_pmainLayout->count()-1,p_regulatorInputLayout);
    }
private slots:
    void slot_run_model();
    void slot_apply();
protected:
    ChartWidgetConfig(QWidget*p_parrent, int=0);
public:
    static ChartWidgetConfig * to_new_velocity(QWidget*p_parrent) { return new ChartWidgetConfig(p_parrent); }
    static ChartWidgetConfig * to_new_theta(QWidget*p_parrent) { return new ChartWidgetConfig(p_parrent, 1); }
    static ChartWidgetConfig * to_new_regulator(QWidget*p_parrent) { return new ChartWidgetConfig(p_parrent, 2); }
};

class ChartWidgetFactory : public QWidget
{
private:
    Q_OBJECT
private:
    enum mode
    {
        VELOCITY,
        THETA,
        REGULATOR
    };
protected:
public:
};

#endif // CHARTWIDGETFACTORY_H
