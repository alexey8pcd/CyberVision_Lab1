#include "mainwindow.h"
#include "ui_mainwindow.h"


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
        delete xKernel;
        delete yKernel;
        resultY.normalize();
        ui->label->setPixmap(QPixmap::fromImage(resultY.toQImage()));
    } else {
        Kernel * gaussKernel = Kernel::createGaussKernel(sigma);
        FImage result = Convolution::apply(image, *gaussKernel, type);
        delete gaussKernel;
        result.normalize();
        ui->label->setPixmap(QPixmap::fromImage(result.toQImage()));
    }
}

void MainWindow::on_bOctaves_clicked() {
    double sigma = ui->dsStartSigma->value();
    EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
    FImage image(original);
    int octavesCount = ui->spOctavesCount->value();
    int levelPerOctave = ui->spLevelPerOctave->value();
    PyramidBuilder builder(octavesCount, levelPerOctave, sigma);
    builder.createOctaves(image, type);

}

void MainWindow::on_bMoravec_clicked() {
    EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
    float threshold = (float)ui->sliderThreshold->value() / 1000.;
    FImage image(original);
    InterestPointsDetector detector(image, type);
    QVector<InterestPointsDetector::InterestPoint*> *points
            = detector.detectMoravec(threshold);
    QImage copy(original);
    for(int i = 0; i < points->size(); ++i) {
        InterestPointsDetector::InterestPoint * point = points->at(i);
        for(int x = -1; x < 1; ++x) {
            for (int y = -1; y < 1; ++y) {
                int px = ImageUtil::handleEdgeEffect(
                             point->x + x, image.getWidth(), type);
                int py = ImageUtil::handleEdgeEffect(
                             point->y + y, image.getHeight(), type);
                copy.setPixel(px, py, 0xFFFF0000);
            }
        }
    }
    delete points;
    ui->label->setPixmap(QPixmap::fromImage(copy));
}

void MainWindow::on_sliderThreshold_valueChanged(int value) {
    float threshold = (float)value / 1000.;
    ui->lThresholdValue->setText(QString::asprintf("%2.3f", threshold));
}

void MainWindow::on_bHarris_clicked() {

}
