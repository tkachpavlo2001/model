#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QThread>


#include "chartwidgetfactory.h"

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT
private:
    QPushButton * _pbutton_velocity_run = nullptr;
    QPushButton * _pbutton_theta_run = nullptr;
    QPushButton * _pbutton_regulator_run = nullptr;
    iWidgetAbstractFactory * _pWidgetFactory = nullptr;

    iChartWidget * _p_ChartWidget = nullptr;
    iChartWidgetConfig * _p_ChartWidgetConfig = nullptr;

    void _to_delete(iWidgetAbstractFactory * p) { if ( p != nullptr) delete p; }
protected:
    MainWindow()
    {
        QWidget * pCentralWidget = new QWidget(this);
        this->setCentralWidget(pCentralWidget);

        QVBoxLayout * pMainLayout = new QVBoxLayout(pCentralWidget);
        pCentralWidget->setLayout(pMainLayout);

        _pbutton_velocity_run = new QPushButton(this);
        _pbutton_velocity_run->setText("Velocity");
        pMainLayout->addWidget(_pbutton_velocity_run);

        _pbutton_theta_run = new QPushButton(this);
        _pbutton_theta_run->setText("Theta");
        pMainLayout->addWidget(_pbutton_theta_run);

        _pbutton_regulator_run = new QPushButton(this);
        _pbutton_regulator_run->setText("Regulator");
        pMainLayout->addWidget(_pbutton_regulator_run);

        connect(_pbutton_velocity_run, &QPushButton::clicked, this, &MainWindow::slot_run_velocity_mode);
        connect(_pbutton_theta_run, &QPushButton::clicked, this, &MainWindow::slot_run_theta_mode);
        connect(_pbutton_regulator_run, &QPushButton::clicked, this, &MainWindow::slot_run_regulator_mode);


    }
    ~MainWindow() override { _to_delete(_pWidgetFactory); };

    void _to_run_subApp()
    {
        _p_ChartWidget = _pWidgetFactory->to_new_ChartWidget(nullptr);
        _p_ChartWidgetConfig = _pWidgetFactory->to_new_ChartWidgetConfig(nullptr);

        //connect(_p_ChartWidgetConfig, &iChartWidgetConfig::signal_run_model, _p_ChartWidget, &iChartWidget::slot_to_run_model);

        qDebug() << "DONE3\n";

        QThread * pThread_run = new QThread(this);
        //_p_ChartWidget->moveToThread(pThread_run);


        //void (QThread::*overloaded_start)(QThread::NormalPriority) = QThread::start;
        connect(_p_ChartWidgetConfig, &iChartWidgetConfig::signal_run_model, pThread_run, [pThread_run]()
        {
            pThread_run->start();
            pThread_run->setPriority(QThread::NormalPriority);
        } );
        connect(pThread_run, &QThread::started, _p_ChartWidget, &iChartWidget::slot_to_run_model);
        connect(_p_ChartWidget, &iChartWidget::signal_to_notify_run_finished, pThread_run, &QThread::quit);
        //connect(_p_ChartWidget, &iChartWidget::signal_to_notify_run_finished, pThread_run, &QThread::deleteLater);

        _p_ChartWidget->show();
        _p_ChartWidgetConfig->show();
    }
private slots:
    void slot_run_velocity_mode() { _to_delete(_pWidgetFactory); _pWidgetFactory = new WidgetFactory_velocity; _to_run_subApp(); }
    void slot_run_theta_mode() { _to_delete(_pWidgetFactory); _pWidgetFactory = new WidgetFactory_theta; _to_run_subApp(); }
    void slot_run_regulator_mode() { _to_delete(_pWidgetFactory); _pWidgetFactory = new WidgetFactory_regulator; _to_run_subApp(); }
public:
    static MainWindow * to_new()
    {
        return new MainWindow();
    }
};


#endif // MAINWINDOW_H
