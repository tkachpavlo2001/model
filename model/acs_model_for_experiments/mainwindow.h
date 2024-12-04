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

#include "chartwidgetfactory.h"

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT
private:
    QPushButton * _pbutton_velocity_run = nullptr;
    QPushButton * _pbutton_theta_run = nullptr;
    QPushButton * _pregulator_run = nullptr;
    iWidgetAbstractFactory * _pWidgetFactory = nullptr;

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

        _pregulator_run = new QPushButton(this);
        _pregulator_run->setText("Regulator");
        pMainLayout->addWidget(_pregulator_run);

        connect(_pbutton_theta_run, &QPushButton::clicked, this, &MainWindow::slot_run_theta_mode);
    }
    ~MainWindow() override { _to_delete(_pWidgetFactory); };

    void _to_run_subApp()
    {
        iChartWidget * p_ChartWidget = _pWidgetFactory->to_new_ChartWidget(nullptr);
        iChartWidgetConfig * p_ChartWidgetConfig = _pWidgetFactory->to_new_ChartWidgetConfig(nullptr);

        QWidget * p_window_1 = new QWidget(this);
        p_window_1->resize(400, 300);
        QWidget * p_window_2 = new QWidget(this);
        p_window_2->resize(400, 300);

        p_ChartWidget->show();
        p_ChartWidgetConfig->show();
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
