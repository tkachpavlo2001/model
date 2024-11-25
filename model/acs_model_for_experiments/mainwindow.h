#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "ui_dialog.h"

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT
public:
    MainWindow()
    {
        QPushButton * button = new QPushButton("Click this one", this);
        button->setGeometry(50, 50, 200, 50);
        connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    }
    //MainWindow(QWidget* arg=nullptr);
    ~MainWindow() override;
private slots:
    void onButtonClicked()
    {
        QMessageBox::information(this, "Message", "The button is pushed");
    }
private:
    //Ui::MainWindow * ui;
};



#endif // MAINWINDOW_H
