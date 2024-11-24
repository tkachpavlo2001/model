#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_dialog.h"

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT
public:
    MainWindow(QWidget* arg=nullptr);
    ~MainWindow() override;
private:
    Ui::MainWindow * ui;
};



#endif // MAINWINDOW_H
