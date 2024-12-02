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

class iChartWidget : public QWidget
{
private:
    Q_OBJECT
protected:
    QBoxLayout * _pMainLayout = nullptr;
    QChartView * _pMainChart = nullptr;
    QChart * _pBackChart = nullptr;
    QSplineSeries * _pSeries = nullptr;
    QLabel * _pInputLabel = nullptr;
    QSlider * _pInputSlider = nullptr;
    QLabel * _pknLabel = nullptr;
    QSlider * _pk0Slider = nullptr;
    QSlider * _pk1Slider = nullptr;
    QLCDNumber * _pOutputNumber = nullptr;
    void _to_init()
    {
        _pMainLayout = new QHBoxLayout(this);

        QVBoxLayout * pInputLayout = new QVBoxLayout;
        _pMainLayout->addLayout(pInputLayout);

        _pInputLabel = new QLabel(this);
        _pInputLabel->setText("NODATA");
        pInputLayout->addWidget(_pInputLabel);

        _pInputSlider = new QSlider(this);
        pInputLayout->addWidget(_pInputSlider);


        QVBoxLayout * pknLayout = new QVBoxLayout;
        _pMainLayout->addLayout(pknLayout);

        _pknLabel = new QLabel(this);
        _pknLabel->setText("k0 k1");
        pknLayout->addWidget(_pknLabel);

        QHBoxLayout * pknSliderLayout = new QHBoxLayout;
        pknLayout->addLayout(pknLayout);

        _pk0Slider = new QSlider(this);
        pInputLayout->addWidget(_pk0Slider);
        _pk1Slider = new QSlider(this);
        pInputLayout->addWidget(_pk1Slider);
        pknLayout->addWidget(_pk0Slider);
        pknLayout->addWidget(_pk1Slider);

        _pMainChart = new QChartView(this);
        _pMainLayout->addWidget(_pMainChart);

        _pOutputNumber = new QLCDNumber(this);
    }
protected:
    iChartWidget(QWidget*p_parent) : QWidget(p_parent) { _to_init(); }
public:
    static iChartWidget * to_new(QWidget*p_parent) { return new iChartWidget(p_parent); }
};
class ChartWidget_velocity : public iChartWidget
{
private:
    ChartWidget_velocity(QWidget*p_parent) : iChartWidget(p_parent)
    {
        _pInputLabel->setText("Voltage");
    }
public:
    static ChartWidget_velocity * to_new(QWidget*p_parent) { return new ChartWidget_velocity(p_parent); }
};
class ChartWidget_theta : public iChartWidget
{
private:
    ChartWidget_theta(QWidget*p_parent) : iChartWidget(p_parent)
    {
        _pInputLabel->setText("Voltage");
    }
public:
    static ChartWidget_theta * to_new(QWidget*p_parent) { return new ChartWidget_theta(p_parent); }
};
class ChartWidget_regulator : public iChartWidget
{
private:
    ChartWidget_regulator(QWidget*p_parent) : iChartWidget(p_parent)
    {
        _pInputLabel->setText("RefSignal");
    }
public:
    static ChartWidget_regulator * to_new(QWidget*p_parent) { return new ChartWidget_regulator(p_parent); }
};

class iChartWidgetConfig : public QWidget
{
private:
    Q_OBJECT
protected:
    QBoxLayout * _pMainLayout = nullptr;
    QLabel * _pInputLabel = nullptr;
    QLineEdit * _pInputEdit = nullptr;
    QLabel * _pk0Label = nullptr;
    QLineEdit * _pk0Edit = nullptr;
    QLabel * _pk1Label = nullptr;
    QLineEdit * _pk1Edit = nullptr;
    QPushButton * _pRunButton = nullptr;
    QPushButton * _pApplyButton = nullptr;
    QLabel * _pRegulatorLabel = nullptr;
    QLineEdit * _pkpEdit = nullptr;
    QLineEdit * _pkiEdit = nullptr;
    QLineEdit * _pkdEdit = nullptr;
private:
    void _to_init()
    {
        _pMainLayout = new QVBoxLayout(this);

        QHBoxLayout * p_primeInputLayout = new QHBoxLayout;
        _pMainLayout->addLayout(p_primeInputLayout);

        _pInputLabel = new QLabel(this);
        _pInputLabel->setText("NODATA");
        p_primeInputLayout->addWidget(_pInputLabel);

        _pInputEdit = new QLineEdit(this);
        p_primeInputLayout->addWidget(_pInputEdit);

        _pk0Label = new QLabel(this);
        _pk0Label->setText("Torque(w) = k0+k1*w = ");
        p_primeInputLayout->addWidget(_pk0Label);

        _pk0Edit = new QLineEdit(this);
        p_primeInputLayout->addWidget(_pk0Edit);

        _pk1Label = new QLabel(this);
        _pk1Label->setText(" + w * ");
        p_primeInputLayout->addWidget(_pk1Label);

        _pk1Edit = new QLineEdit(this);
        p_primeInputLayout->addWidget(_pk1Edit);

        QHBoxLayout * pButtonsLayout = new QHBoxLayout;
        _pMainLayout->addLayout(pButtonsLayout);

        _pRunButton = new QPushButton(this);
        _pRunButton->setText("Run");
        _pApplyButton = new QPushButton(this);
        _pRunButton->setText("Apply");
        pButtonsLayout->addWidget(_pApplyButton);
        pButtonsLayout->addWidget(_pRunButton);
    }
private slots:
    void slot_run_model();
    void slot_apply();
protected:
    iChartWidgetConfig(QWidget*p_parent) : QWidget(p_parent) { _to_init(); }
    virtual ~iChartWidgetConfig() = 0;
public:
    //static iChartWidgetConfig * to_new(QWidget*p_parent) { return new iChartWidgetConfig(p_parent); }
};
class ChartWidgetConfig_velocity : iChartWidgetConfig
{
private:
    ChartWidgetConfig_velocity(QWidget*p_parent) : iChartWidgetConfig(p_parent)
    {
        _pInputLabel->setText("Velocity: ");
        ;
    }
public:
    static ChartWidgetConfig_velocity * to_new(QWidget*p_parent) { return new ChartWidgetConfig_velocity(p_parent); }
};
class ChartWidgetConfig_theta : iChartWidgetConfig
{
private:
    ChartWidgetConfig_theta(QWidget*p_parent) : iChartWidgetConfig(p_parent)
    {
        _pInputLabel->setText("Theta: ");
        ;
    }
public:
    static ChartWidgetConfig_theta * to_new(QWidget*p_parent) { return new ChartWidgetConfig_theta(p_parent); }
};
class ChartWidgetConfig_regulator : iChartWidgetConfig
{
private:
    ChartWidgetConfig_regulator(QWidget*p_parent) : iChartWidgetConfig(p_parent)
    {
        _pInputLabel->setText("Theta: ");

        QHBoxLayout * pRegulatorInputLayout = new QHBoxLayout;
        _pMainLayout->insertLayout(_pMainLayout->count()-1, pRegulatorInputLayout);

        _pRegulatorLabel = new QLabel(this);
        _pRegulatorLabel->setText("Regulator koefs (kp,ki,kd) :");
        pRegulatorInputLayout->addWidget(_pRegulatorLabel);

        _pkpEdit = new QLineEdit(this);
        pRegulatorInputLayout->addWidget(_pkpEdit);

        _pkiEdit = new QLineEdit(this);
        pRegulatorInputLayout->addWidget(_pkiEdit);

        _pkdEdit = new QLineEdit(this);
        pRegulatorInputLayout->addWidget(_pkdEdit);
    }
public:
    static ChartWidgetConfig_regulator * to_new(QWidget*p_parent) { return new ChartWidgetConfig_regulator(p_parent); }
};



/*
class core : public QWidget
{
private:
    Q_OBJECT
public:
    iChartWidget * p_iChartWidget = nullptr;
    iChartWidgetConfig * p_iChartWidgetConfig = nullptr;
    core(QWidget * p_parent) : QWidget(p_parent) {}
};

class ChartWidgetAbstractFactory : public QWidget
{
private:
    Q_OBJECT
private:
    enum mode
    {
        VELOCITY = 0,
        THETA = 1,
        REGULATOR = 2
    } _mode;
    core * _to_new(QWidget * p_parent, int mode, int n)
    {
        switch(mode)
        {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            QMessageBox * pError = new QMessageBox(this);
            pError->setIcon(QMessageBox::Critical);
            pError->setDefaultButton(QMessageBox::Close);
            pError->setText("iChartWidgetFactory problem");
            pError->show();
            break;
        }

    }
protected:
    ChartWidgetAbstractFactory(QWidget * p_parent, int n = 0) : QWidget(p_parent) { _mode = mode(n); }
public:
    static ChartWidgetAbstractFactory * to_new_velocity(QWidget * p_parent) { return new ChartWidgetAbstractFactory(p_parent); }
    static ChartWidgetAbstractFactory * to_new_theta(QWidget * p_parent) { return new ChartWidgetAbstractFactory(p_parent, 1); }
    static ChartWidgetAbstractFactory * to_new_regulator(QWidget * p_parent) { return new ChartWidgetAbstractFactory(p_parent, 2); }
    iChartWidget * to_new_chart() { _to_new(this, _mode, 0); }
    iChartWidgetConfig * to_new_config() { _to_new(this, _mode, 1); }
};
*/
#endif // CHARTWIDGETFACTORY_H
