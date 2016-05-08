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
}


void MainWindow::on_bLoadImage_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выбрать изображение");
    this->original = QImage(fileName).convertToFormat(QImage::Format_ARGB32);
    ui->label->setPixmap(QPixmap::fromImage(original));
}

void MainWindow::on_bSobelX_clicked() {
    EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
    float sobelXValues[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 2};
    Kernel sobelX(sobelXValues, 3);
    FImage image(original);
    FImage result = Convolution::apply(image, sobelX, type);
    result.normalize();
    ui->label->setPixmap(QPixmap::fromImage(result.toQImage()));
}

void MainWindow::on_bSobelY_clicked() {
    EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
    float sobelYValues[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    Kernel sobelY(sobelYValues, 3);
    FImage image(original);
    FImage result = Convolution::apply(image, sobelY, type);
    result.normalize();
    ui->label->setPixmap(QPixmap::fromImage(result.toQImage()));
}

void MainWindow::on_bSobelXY_clicked() {
    EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
    FImage image(original);

    float sobelXValues[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 2};
    Kernel sobelX(sobelXValues, 3);
    FImage resultX = Convolution::apply(image, sobelX, type);
    resultX.normalize();

    float sobelYValues[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    Kernel sobelY(sobelYValues, 3);
    FImage resultY = Convolution::apply(image, sobelY, type);
    resultY.normalize();

    FImage result(resultX.getWidth(), resultY.getHeight());
    for(int x = 0; x < resultX.getWidth(); ++x) {
        for(int y = 0; y < resultX.getHeight(); ++y) {
            float valX = resultX.getValue(x, y);
            float valY = resultY.getValue(x, y);
            result.setValue(x, y, sqrtf(valX * valX + valY * valY));
        }
    }
    result.normalize();
    ui->label->setPixmap(QPixmap::fromImage(result.toQImage()));
}

void MainWindow::on_sliderRadius_valueChanged(int value)
{
    ui->lRadius->setText(QString::number(value));
}

void MainWindow::on_bGauss_clicked(){
    const double sigma = ui->sliderRadius->value();
    EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
    FImage image(original);
    if(ui->chbSeparable->isChecked()){
        Kernel * xKernel = Kernel::createGaussSeparateKernelX(sigma);
        FImage resultX = Convolution::apply(image, *xKernel, type);

        Kernel * yKernel = Kernel::createGaussSeparateKernelY(sigma);
        FImage resultY = Convolution::apply(resultX, *yKernel, type);

        resultY.normalize();
        ui->label->setPixmap(QPixmap::fromImage(resultY.toQImage()));
    } else {
        Kernel * gaussKernel = Kernel::createGaussKernel(sigma);
        FImage result = Convolution::apply(image, *gaussKernel, type);
        result.normalize();
        ui->label->setPixmap(QPixmap::fromImage(result.toQImage()));
    }

}

void MainWindow::on_bOctaves_clicked() {
//    double sigma = ui->dsStartSigma->value();
//    int octavesCount = ui->spOctavesCount->value();
//    int levelPerOctave = ui->spLevelPerOctave->value();
//    if(handler != NULL){
//        PyramidBuilder builder(octavesCount, levelPerOctave, sigma);
//        QImage image = handler->getImage();
//        builder.createOctaves(image);
//    }
}
