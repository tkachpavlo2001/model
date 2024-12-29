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
#include <QTextEdit>


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
        this->setWindowTitle("List of available models");
        this->setFixedHeight(150);
        this->resize(400,400);

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

        connect(_pbutton_velocity_run, &QPushButton::clicked, this, &MainWindow::_slot_run_velocity_mode);
        connect(_pbutton_theta_run, &QPushButton::clicked, this, &MainWindow::_slot_run_theta_mode);
        connect(_pbutton_regulator_run, &QPushButton::clicked, this, &MainWindow::_slot_run_regulator_mode);

        QHBoxLayout * pInfoLayout = new QHBoxLayout();
        pMainLayout->addLayout(pInfoLayout);
        QPushButton * pAskButton = new QPushButton(this);
        pAskButton->setText("?");
        QPushButton * pInfoButton = new QPushButton(this);
        pInfoButton->setText("info");
        pInfoLayout->addWidget(pAskButton);
        pInfoLayout->addWidget(pInfoButton);

        connect(pAskButton, &QPushButton::clicked, this, &MainWindow::_slot_ask_activated);
        connect(pInfoButton, &QPushButton::clicked, this, &MainWindow::_slot_info_activated);
    }
    ~MainWindow() override { _to_delete(_pWidgetFactory); };

    void _to_run_subApp()
    {
        _p_ChartWidget = _pWidgetFactory->to_new_ChartWidget(nullptr);
        _p_ChartWidgetConfig = _pWidgetFactory->to_new_ChartWidgetConfig(nullptr);
        config shared_configs;

        _p_ChartWidget->to_set_config(shared_configs);
        _p_ChartWidgetConfig->to_set_config(shared_configs);
        _p_ChartWidgetConfig->to_set_init_input();
        _p_ChartWidgetConfig->slot_apply();

        qDebug() << "DONE3\n";

        connect(_p_ChartWidgetConfig, &iChartWidgetConfig::signal_run_model, _p_ChartWidget, &iChartWidget::slot_to_run_model);
        connect(_p_ChartWidgetConfig, &iChartWidgetConfig::signal_model_updated, _p_ChartWidget, &iChartWidget::slot_to_update_model);
        connect(_p_ChartWidget, &iChartWidget::signal_to_notify_run_finished, _p_ChartWidgetConfig, &iChartWidgetConfig::slot_run_finished);

        _p_ChartWidget->show();
        _p_ChartWidgetConfig->show();
    }
private slots:
    void _slot_run_velocity_mode() { _to_delete(_pWidgetFactory); _pWidgetFactory = new WidgetFactory_velocity; _to_run_subApp(); }
    void _slot_run_theta_mode() { _to_delete(_pWidgetFactory); _pWidgetFactory = new WidgetFactory_theta; _to_run_subApp(); }
    void _slot_run_regulator_mode() { _to_delete(_pWidgetFactory); _pWidgetFactory = new WidgetFactory_regulator; _to_run_subApp(); }
    void _slot_ask_activated()
    {
        QTextEdit * p = new QTextEdit;
        p->setWindowTitle("Help Window");
        p->setReadOnly(true);
        p->setAlignment(Qt::AlignTop);
        p->setFixedSize(600, 200);
        p->setText(
                    QString("\'") +
                    QString(_pbutton_velocity_run->text()) +
                    QString("\'") +
                    QString("\t is the mode without a PID controller and angular velocity fixation.\n\n") +
                    QString("\'") +
                    QString(_pbutton_theta_run->text()) +
                    QString("\'") +
                    QString("\t is the mode without a PID controller and angular displacement fixation.\n\n") +
                    QString("\'") +
                    QString(_pbutton_regulator_run->text()) +
                    QString("\'") +
                    QString("\t is the mode with full completion, ACS model with PID controller.\n\n") +

                    QString("\n\nMore in the information window")
                    );
        p->show();
    }
    void _slot_info_activated()
    {
        QTextEdit * p = new QTextEdit;
        p->setWindowTitle("Information about Application");
        p->setReadOnly(true);
        p->setAlignment(Qt::AlignTop);
        p->setFixedSize(1000, 500);
        p->setText(
                    "This application provides an interactive approach to acquire hands-on skills in the experimental tuning of PID controllers.\n"
                   "Also, it facilitates users in evaluating the effectiveness of the automated control system by providing the opportunity to run and estimate the model with the regulator excluded.\n"
                   "Even a user without technical education is capable of understanding basics within half an hour with the explanations provided by those educated in this field, as has been observed.\n\n"
                   "Automated control system in this application is based on the electromotive drive as the controlled object. The automated control system is dedicated to sustaining a certain angular displacement value of the engine shaft. The regulating value is the voltage applied to the motor terminals. The electric motor is a direct current engine with independent windings. Consequently, the source of electricity is a DC type.\n\n"
                   "There are two ways the model can be run:\n"
                   "1. The model in full completion.\n"
                   "2. The model with no PID controller.\n "
                   "The second way has two different modes that differ in the value fixed as the output from the system. One of them, the same as in the main mode with full completion, is the angular displacement, and the second one is the angular velocity.\n"
                   "Therefore, here are three modes of the model run as:\n"
                   "1. Full completion.\n"
                   "2. Without PID controller and angular displacement fixation.\n"
                   "3. Without PID controller and angular velocity fixation.\n\n"
                   "The main application window allows a user to choose the mode.\n\n"
                   "When the mode has been chosen, two windows are provided for use. The chart window plots data in real-time, and the sliders allow a user to change the percentage of the main values applied to the model. These main values are defined in another window. This window is intended to define the main values for the run. Their definition serves as a limitation because the chart window provides the ability to use the percentage range only within the 0-100% span.\n\n"
                   "The value definition window defines the following values for each mode:\n"
                   "1. Input value\n"
                   "2. Torque coefficients\n"
                   "In full completion mode, a reference value is used as the input value, while in other modes, a voltage is used as the input value. The torque coefficients have the same 'k0' and 'k1' in every mode, and they correspond to the equation:\n"
                   "'T(w) = k0 + k1 * w'\n"
                   ", where 'w' is angular velocity;\n"
                   "'k0' is the constant part;\n"
                   "'k1' is proportionally related to angular velocity part.\n\n"
                   "The full completion mode requires the definition of the 'kp', 'ki' and 'kd' values that correspond to the equation:\n"
                   "'u(t) = kp * e(t) + ki * integral( e(t), dt ) + kd * derivative( e(t), dt)'\n"
                   ", where 'kp' is the proportional coefficient;\n"
                   "'ki' is the integral coefficient;\n"
                   "'kd' is the derivative coefficient;\n"
                   "The regulator tuning consists of the finding appropriate values for these coefficients that satisfy the requirements.\n\n"
                   "The chart plot window allows a user to change input value, 'k0', and 'k1' with the sliders.\n\n"
                   "All changes can be applied in real time, while the model is processing and the chart is plotting. This opportunity allows for the implementation the interactive approach that has been mentioned.\n\n"
                   "The model list has been composed from the simpler models, easiest to visualise, to the more complex ones:\n"
                   "1. Without a PID controller and angular velocity fixation.\n"
                   "2. Without a PID controller and angular displacement fixation.\n"
                   "3. Full completion.\n"
                   "This order is designed to gradually introduce this concept with step-by-step increasing difficulty to understand and visualize.\n\n"
                   "In the last mode on the list, it is recommended not to use the next coefficients until a user has understood the concept of the previous.\n"
                   "For example, start with only 'kp'. When the user has understood its impact, he can start changing 'ki'. The last 'kd' coefficient can be used after the 'kp' and 'ki' regulation is clearly understandable to the user.\n\n"
                   "The theory must be under review while using this application for the best study experience.\n\n"
                   "The application was developed by Pavlo Tkach, a former student of the Faculty of Electrical Engineering at Kryvyi Rih National University. The application uses the program modules developed during the qualification work on the theme \"Research of modern software methods of the classic PID controller improvement and modifications\" for a master's degree in software engineering at the Faculty of Information Technologies at Kryvyi Rih National University.\n\n"
                   "The application is intended to be used by the Faculty of Electrical Engineering and anyone who is interested in PID controller concepts.\n\n"
                   "This project is licensed under the GNU General Public License v3.0 - see the LICENSE.txt file for details.\n"
                   "https://github.com/tkachpavlo2001/model\n"
                   );
        p->show();
    }
public:
    static MainWindow * to_new()
    {
        return new MainWindow();
    }
};


#endif // MAINWINDOW_H
