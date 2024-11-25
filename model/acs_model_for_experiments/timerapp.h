#ifndef TIMERAPP_H
#define TIMERAPP_H

#include<QWidget>
#include<QLabel>
#include<QLayout>
#include<QPushButton>
#include<QTimer>
#include<QMessageBox>


class TimerApp : public QWidget
{
    Q_OBJECT
public:
    TimerApp(QWidget * parent = nullptr) : QWidget(parent), counter(0)
    {
        QLayout * mainLayout = new QVBoxLayout;

        p_timerLabel = new QLabel("Time: 0", this);
        mainLayout->addWidget(p_timerLabel);

        QPushButton * startButton = new QPushButton("Start the timer", this);
        mainLayout->addWidget(startButton);

        QPushButton * resetButton = new QPushButton("Reset the timer", this);
        mainLayout->addWidget(resetButton);

        p_timer = new QTimer(this);

        connect(p_timer, &QTimer::timeout, this, &TimerApp::updateTimer);
        connect(startButton, &QPushButton::clicked, this, &TimerApp::startTimer);
        connect(resetButton, &QPushButton::clicked, this, &TimerApp::resetTimer);
        connect(this, &TimerApp::onThresholdReached, this, &TimerApp::onThresholdReached_action);

        setLayout(mainLayout);
    }
signals:
    void onThresholdReached();
private slots:
    void startTimer()
    {
        if (!p_timer->isActive()) p_timer->start(1000);
    }
    void updateTimer()
    {
        counter++;
        p_timerLabel->setText("Time: " + QString::number(counter));
        if (counter >= 10) emit onThresholdReached();
    }
    void resetTimer()
    {
        p_timer->stop();
        counter = 0;
        p_timerLabel->setText("Time: 0");
    }
    void onThresholdReached_action()
    {
        QMessageBox::information(this, "Notion", QString(QString::number(counter) + "th passed"));
    }
private:
    QTimer * p_timer;
    QLabel * p_timerLabel;
    int counter;
};

#endif // TIMERAPP_H
