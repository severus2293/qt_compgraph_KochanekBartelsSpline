#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tensionSlider_valueChanged(int value)
{
    this->ui->widget->tension = (double) value / 100.0;
}

void MainWindow::on_biasSlider_valueChanged(int value)
{
     this->ui->widget->bias = (double) value / 100.0;
}

void MainWindow::on_continuitySlider_valueChanged(int value)
{
     this->ui->widget->continuity = (double) value / 100.0;
}
