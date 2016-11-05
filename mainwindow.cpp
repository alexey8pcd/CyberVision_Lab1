#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "siftdescriptorssearcher.h"
#include "descriptorsmatcher.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    original = nullImage;
    img2 = nullImage;
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_bLoadImage_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выбрать изображение");
    try {
        this->original = QImage(fileName).convertToFormat(QImage::Format_ARGB32);
        ui->imageLabel->setPixmap(QPixmap::fromImage(original));
    } catch (...) {
        original = nullImage;
    }
}

void MainWindow::on_bSobelX_clicked() {
    if(original != nullImage){
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        FImage image(original);
        Kernel sobelX = Kernel::createSobelKernelX();
        Convolution conv = Convolution(sobelX, type);
        FImage result = conv.apply(image);
        result.normalize();
        ui->imageLabel->setPixmap(QPixmap::fromImage(result.toQImage()));
    }
}

void MainWindow::on_bSobelY_clicked() {
    if (original != nullImage){
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        FImage image(original);
        Kernel sobelY = Kernel::createSobelKernelY();
        Convolution conv = Convolution(sobelY, type);
        FImage result = conv.apply(image);
        result.normalize();
        ui->imageLabel->setPixmap(QPixmap::fromImage(result.toQImage()));
    }
}

void MainWindow::on_bSobelXY_clicked() {
    if(original != nullImage){
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        FImage image(original);
        Kernel sobelX = Kernel::createSobelKernelX();
        Convolution convX = Convolution(sobelX, type);
        FImage resultX = convX.apply(image);
        resultX.normalize();

        Kernel sobelY = Kernel::createSobelKernelY();
        Convolution convY = Convolution(sobelY, type);
        FImage resultY = convY.apply(image);
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
        ui->imageLabel->setPixmap(QPixmap::fromImage(result.toQImage()));
    }
}

void MainWindow::on_sliderRadius_valueChanged(int value) {
    ui->lRadius->setText(QString::number(value));
}

void MainWindow::on_bGauss_clicked(){
    if(original != nullImage){
        const double sigma = ui->sliderRadius->value();
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        FImage image(original);
        if(ui->chbSeparable->isChecked()){
            Kernel xKernel = Kernel::createGaussSeparateKernelX(sigma);
            Convolution convX = Convolution(xKernel, type);
            FImage resultX = convX.apply(image);

            Kernel yKernel = Kernel::createGaussSeparateKernelY(sigma);
            Convolution convY = Convolution(yKernel, type);
            FImage resultY = convY.apply(resultX);
            resultY.normalize();
            ui->imageLabel->setPixmap(QPixmap::fromImage(resultY.toQImage()));
        } else {
            Kernel gaussKernel = Kernel::createGaussKernel(sigma);
            Convolution conv = Convolution(gaussKernel, type);
            FImage result = conv.apply(image);
            result.normalize();
            ui->imageLabel->setPixmap(QPixmap::fromImage(result.toQImage()));
        }
    }
}

void MainWindow::on_bOctaves_clicked() {
    if(original != nullImage){
        ui->lState->setText("");
        double startSigma = ui->dsStartSigma->value();
        double sigmaAlias = ui->dsSigmaA->value();
        if(sigmaAlias > startSigma){
            QMessageBox box;
            QString text = "Значение параметра сигма начальная";
            text.append(" не может быть меньше значения сигма сглаженная");
            box.setText(text);
            box.exec();
        }
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        FImage image(original);
        int levelPerOctave = ui->spLevelPerOctave->value();
        PyramidBuilder builder(image, type, levelPerOctave,
                               sigmaAlias, startSigma);
        builder.createOctaves();
        builder.save();
        ui->lState->setText("Завершено");
    }
}

void MainWindow::on_bMoravec_clicked() {
    if(original != nullImage){
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        float threshold = (float)ui->sliderThreshold->value() / 1000.;
        FImage image(original);
        InterestPointsDetector detector(image, type, 2);
        if(ui->chbFilter->isChecked()){
            detector.enableFilter(ui->spinInterestPointsCount->value());
        }
        vector<InterestPoint> points = detector.detectMoravec(threshold);
        QImage copy(original);
        foreach (const InterestPoint& point, points) {
            for(int x = -1; x < 1; ++x) {
                for (int y = -1; y < 1; ++y) {
                    int px = ImageUtil::handleEdgeEffect(
                                 point.x + x, image.getWidth(), type);
                    int py = ImageUtil::handleEdgeEffect(
                                 point.y + y, image.getHeight(), type);
                    if(ImageUtil::insideImage(px, py)){
                        copy.setPixel(px, py, 0xFFFF0000);
                    }
                }
            }
        }
        ui->imageLabel->setPixmap(QPixmap::fromImage(copy));
    }
}

void MainWindow::on_sliderThreshold_valueChanged(int value) {
    float threshold = (float)value / 1000.;
    ui->lThresholdValue->setText(QString::asprintf("%2.3f", threshold));
}

void MainWindow::on_bHarris_clicked() {
    if(original != nullImage){
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        float threshold = (float)ui->sliderThreshold->value() / 1000.;
        FImage image(original);
        InterestPointsDetector detector(image, type);
        if(ui->chbFilter->isChecked()){
            detector.enableFilter(ui->spinInterestPointsCount->value());
        }
        vector<InterestPoint> points = detector.detectHarris(threshold);
        QImage copy(original);
        foreach (const InterestPoint& point, points) {
            for(int x = -1; x < 1; ++x) {
                for (int y = -1; y < 1; ++y) {
                    int px = ImageUtil::handleEdgeEffect(
                                 point.x + x, image.getWidth(), type);
                    int py = ImageUtil::handleEdgeEffect(
                                 point.y + y, image.getHeight(), type);
                    if(ImageUtil::insideImage(px, py)){
                        copy.setPixel(px, py, 0xFFFF0000);
                    }
                }
            }
        }
        ui->imageLabel->setPixmap(QPixmap::fromImage(copy));
    }
}

void MainWindow::on_bLoadImg2_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выбрать изображение");
    this->img2 = QImage(fileName).convertToFormat(QImage::Format_ARGB32);
    if(original != nullImage){
        int height = original.height() > img2.height()
                     ? original.height() : img2.height();
        QImage* result = new QImage(original.width() + img2.width(), height,
                      QImage::Format_ARGB32);
        QPainter painter(result);
        painter.drawImage(0, 0, original);
        painter.drawImage(original.width(), 0, img2);
        ui->imageLabel->setPixmap(QPixmap::fromImage(*result));
    }
}

void MainWindow::on_bSearchDescriptors_clicked() {
    if(original != nullImage && img2 !=  nullImage){
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        float threshold = (float)ui->sliderThreshold->value() / 1000.;
        FImage image1(original);
        FImage image2(img2);
        InterestPointsDetector detector1(image1, type);
        InterestPointsDetector detector2(image2, type);        
        detector1.enableFilter(ui->spinMaxDescSize->value());
        detector2.enableFilter(ui->spinMaxDescSize->value());
        const double sigma = ui->sliderRadius->value();
        vector<InterestPoint> points1 = detector1.detectHarris(threshold);
        vector<InterestPoint> points2 = detector2.detectHarris(threshold);

        vector<Descriptor<SIFT_DESC_SIZE>> descriptors1
                = search(image1,
                    points1, type, ui->chbGaussSmooth->isChecked(), sigma);

        vector<Descriptor<SIFT_DESC_SIZE>> descriptors2 = search(image2,
                    points2, type, ui->chbGaussSmooth->isChecked(), sigma);

        vector<pair<Descriptor<SIFT_DESC_SIZE>,
                Descriptor<SIFT_DESC_SIZE>>> pairs =
                associate(descriptors1, descriptors2);
        int height = original.height() > img2.height()
                     ? original.height() : img2.height();
        QImage* result = new QImage(original.width() + img2.width(), height,
                      QImage::Format_ARGB32);
        QPainter painter(result);
        painter.drawImage(0, 0, original);
        painter.drawImage(original.width(), 0, img2);        
        for (int i = 0; i < pairs.size(); ++i) {
            QColor color = QColor((5 * i) % 256,
                                  (11 * i) % 256,
                                  (128 + 23 * i) % 256);
            painter.setPen(color);
            pair<Descriptor<SIFT_DESC_SIZE>,
                    Descriptor<SIFT_DESC_SIZE>> p= pairs.at(i);
            const Descriptor<SIFT_DESC_SIZE> d1 = p.first;
            const Descriptor<SIFT_DESC_SIZE> d2 = p.second;
            painter.drawRect(d1.getX(), d1.getY(), 3, 3);
            painter.drawRect(d2.getX() + img2.width(), d2.getY(), 3, 3);
            painter.drawLine(d1.getX(), d1.getY(),
                             d2.getX() + img2.width(), d2.getY());
        }
        ui->imageLabel->setPixmap(QPixmap::fromImage(*result));
    }
}
