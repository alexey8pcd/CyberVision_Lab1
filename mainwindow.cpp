#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtGui"
#include "QFileDialog"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
    delete handler;
}


void MainWindow::on_bLoadImage_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выбрать изображение");
    if(handler == NULL){
        if (!fileName.isEmpty()){
            handler = new ImageHandler(fileName);
        }
    } else {
        if (!fileName.isEmpty()){
            handler->setImage(fileName);
        }
    }
    ui->label->setPixmap(handler->getOriginal());
}

void MainWindow::on_bSobelX_clicked() {
    if(handler != NULL){
        ui->label->setPixmap(handler->getFilteredSobelX());
    }
}

void MainWindow::on_bSobelY_clicked() {
    if(handler != NULL){
        ui->label->setPixmap(handler->getFilteredSobelY());
    }
}

void MainWindow::on_bSobelXY_clicked()
{
    if(handler != NULL){
        ui->label->setPixmap(handler->getFilteredSobel());
    }
}

void MainWindow::on_sliderRadius_valueChanged(int value)
{
    ui->lRadius->setText(QString::number(value));
}

void MainWindow::on_bGauss_clicked()
{
    const double sigma = ui->sliderRadius->value();
    if(handler != NULL){
        ui->label->setPixmap(handler->getFilteredGauss(sigma));
    }
}
