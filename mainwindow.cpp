#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtGui"
#include "imageutil.h"
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
    if(handler == NULL){
        QString fileName = QFileDialog::getOpenFileName(this, "Выбрать изображение");
        if (!fileName.isEmpty()){
            handler = new ImageHandler(fileName);
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
