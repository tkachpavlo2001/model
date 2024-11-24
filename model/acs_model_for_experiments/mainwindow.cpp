#include "mainwindow.h"


MainWindow::MainWindow(QWidget* arg) : QMainWindow(arg), ui(new Ui::MainWindow)
{
    setWindowTitle("Start configurations");
    ui->setupUi(this);
}
MainWindow::~MainWindow() {}
