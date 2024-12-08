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
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

#include <thread>

#include "registrator.h"
#include "experiment_executor.h"

class iChartWidget : public QWidget
{
private:
    Q_OBJECT
protected:
    QBoxLayout * _pMainLayout = nullptr;
    QChartView * _pMainChart = nullptr;
    QChart * _pBackChart = nullptr;
    QXYSeries * _pSeries = nullptr;
    QXYSeries * _pSeries_def = nullptr;
    QValueAxis * _pAxisX = nullptr;
    QValueAxis * _pAxisY = nullptr;
    QLabel * _pInputLabel = nullptr;
    QSlider * _pInputSlider = nullptr;
    QLabel * _pknLabel = nullptr;
    QSlider * _pk0Slider = nullptr;
    QSlider * _pk1Slider = nullptr;
    QLCDNumber * _pOutputNumber = nullptr;

    Registrator_qt * _pRegistrator = nullptr;

    std::shared_ptr<Reference_signal_definder_static> _definder = nullptr;
    std::shared_ptr<PID_regulator> _regulator = nullptr;
    std::shared_ptr<DC_source_inerted> _source = nullptr;
    std::shared_ptr<DC_engine> _process = nullptr;
    std::shared_ptr<Automated_control_system_paralleled> _acs_model = nullptr;
    std::shared_ptr<Experiment_executor_interface> _experiment = nullptr;

    void _to_back_init()
    {
        _pBackChart = new QChart();//
        _pMainChart->setChart(_pBackChart);
        _pMainChart->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _pMainChart->setCacheMode(QGraphicsView::CacheNone);



        _pRegistrator = Registrator_qt::to_new(this);
        _pRegistrator->to_set_chart(this);

        _pSeries = new QLineSeries(this);
        _pBackChart->addSeries(_pSeries);
        _pRegistrator->to_set_series(_pSeries);

        _pSeries_def = new QLineSeries(this);
        _pBackChart->addSeries(_pSeries_def);
        _pRegistrator->to_set_series_additional(_pSeries_def);

        _pAxisX = new QValueAxis(this);
        _pAxisY = new QValueAxis(this);
        _pAxisX->setRange(0,50);//?
        _pAxisY->setRange(-10,50);//?
        _pAxisX->setLabelFormat("%.2f");
        _pAxisY->setLabelFormat("%.2f");

        _pBackChart->addAxis(_pAxisX, Qt::AlignBottom);
        _pBackChart->addAxis(_pAxisY, Qt::AlignLeft);

        _pSeries->attachAxis(_pAxisX);
        _pSeries->attachAxis(_pAxisY);

        _pSeries_def->attachAxis(_pAxisX);
        _pSeries_def->attachAxis(_pAxisY);

        _pMainChart->setRenderHint(QPainter::Antialiasing);



    }
    void _to_model_init();
    void _to_front_init()
    {
        _pMainLayout = new QHBoxLayout(this);
        this->setLayout(_pMainLayout);
        this->resize(400,300);

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
        pknLayout->addLayout(pknSliderLayout);

        _pk0Slider = new QSlider(this);
        pknSliderLayout->addWidget(_pk0Slider);
        _pk1Slider = new QSlider(this);
        pknSliderLayout->addWidget(_pk1Slider);

        _pMainChart = new QChartView(this);
        _pMainChart->setRenderHint(QPainter::Antialiasing);
        _pMainLayout->addWidget(_pMainChart);

        _pOutputNumber = new QLCDNumber(this);
        _pMainLayout->addWidget(_pOutputNumber);
    }
    void _to_init()
    {
        _to_front_init();
        _to_back_init();
        _to_model_init();

        //connect(_pSeries, &QSplineSeries::pointAdded, this, &iChartWidget::slot_to_update_chart);
        connect(_pSeries, &QSplineSeries::pointAdded, this, &iChartWidget::slot_to_update_chart);
        //connect(this, &iChartWidget::signal_to_update_chart, this, &iChartWidget::slot_to_update_chart);
    }

    void _to_reset_chart();

    virtual void _to_run() = 0;
signals:
    void signal_to_update_chart();
    void signal_to_notify_run_finished();
private slots:
    void slot_to_update_chart()
    {
        //std::thread b ([this](){ _pMainChart->update(); });
        //b.join();
    }
    void slot_to_update_model() {}
protected:
    iChartWidget(QWidget*p_parent) : QWidget(p_parent) { _to_init(); }
    ~iChartWidget();
public slots:
    void slot_to_run_model()
    {
        qDebug() << "DONE2\n";
        std::thread a ([this](){ _to_run(); emit signal_to_notify_run_finished();} );
        a.detach();
    }
public:
    void to_update_chart() { _pMainChart->update(); }
    // static iChartWidget * to_new(QWidget*p_parent) { return new iChartWidget(p_parent); }
};
class ChartWidget_velocity : public iChartWidget
{
private:
    ChartWidget_velocity(QWidget*p_parent) : iChartWidget(p_parent)
    {
        _pInputLabel->setText("Voltage");
    }
    void _to_run() override;
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
    void _to_run() override;
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
    void _to_run() override;
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
        this->setLayout(_pMainLayout);

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
        _pApplyButton->setText("Apply");
        pButtonsLayout->addWidget(_pApplyButton);
        pButtonsLayout->addWidget(_pRunButton);

        connect(_pRunButton, &QPushButton::clicked, this, &iChartWidgetConfig::slot_run_model);
    }
signals:
    void signal_run_model();
    void signal_model_updated();
    void slot_update_chart();
private slots:
    void slot_run_model() { emit signal_run_model(); }
    void slot_apply() {}
protected:
    iChartWidgetConfig(QWidget*p_parent) : QWidget(p_parent) { _to_init(); }
    virtual ~iChartWidgetConfig() = 0;
public:
    //static iChartWidgetConfig * to_new(QWidget*p_parent) { return new iChartWidgetConfig(p_parent); }
};
class ChartWidgetConfig_velocity : public iChartWidgetConfig
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
class ChartWidgetConfig_theta : public iChartWidgetConfig
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
class ChartWidgetConfig_regulator : public iChartWidgetConfig
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

class iWidgetAbstractFactory
{
protected:
    iWidgetAbstractFactory() {}
public:
    virtual ~iWidgetAbstractFactory() = 0;
    virtual iChartWidget * to_new_ChartWidget(QWidget*p) = 0;
    virtual iChartWidgetConfig * to_new_ChartWidgetConfig(QWidget*p) = 0;
};

template<class ChartWidget, class ChartWidgetConfig>
class bWidgetAbstractFactory : public iWidgetAbstractFactory
{
protected:
    bWidgetAbstractFactory() {}
public:
    virtual ~bWidgetAbstractFactory() {}
    iChartWidget * to_new_ChartWidget(QWidget*p) override { return ChartWidget::to_new(p); }
    iChartWidgetConfig * to_new_ChartWidgetConfig(QWidget*p) override { return ChartWidgetConfig::to_new(p); }
};

class WidgetFactory_velocity : public bWidgetAbstractFactory<ChartWidget_velocity, ChartWidgetConfig_velocity> {};

class WidgetFactory_theta : public bWidgetAbstractFactory<ChartWidget_theta, ChartWidgetConfig_theta> {};

class WidgetFactory_regulator : public bWidgetAbstractFactory<ChartWidget_regulator, ChartWidgetConfig_regulator> {};


#endif // CHARTWIDGETFACTORY_H
