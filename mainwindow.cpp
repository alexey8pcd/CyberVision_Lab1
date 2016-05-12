#include "mainwindow.h"
#include "ui_mainwindow.h"


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
        ui->label->setPixmap(QPixmap::fromImage(original));
    } catch (...) {
        original = nullImage;
    }
}

void MainWindow::on_bSobelX_clicked() {
    if(original != nullImage){
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        FImage image(original);
        Kernel sobelX = Kernel::createSobelKernelX();
        FImage result = Convolution::apply(image, sobelX, type);
        result.normalize();
        ui->label->setPixmap(QPixmap::fromImage(result.toQImage()));
    }
}

void MainWindow::on_bSobelY_clicked() {
    if (original != nullImage){
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        FImage image(original);
        Kernel sobelY = Kernel::createSobelKernelY();
        FImage result = Convolution::apply(image, sobelY, type);
        result.normalize();
        ui->label->setPixmap(QPixmap::fromImage(result.toQImage()));
    }
}

void MainWindow::on_bSobelXY_clicked() {
    if(original != nullImage){
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        FImage image(original);
        Kernel sobelX = Kernel::createSobelKernelX();
        FImage resultX = Convolution::apply(image, sobelX, type);
        resultX.normalize();

        Kernel sobelY = Kernel::createSobelKernelY();
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
            FImage resultX = Convolution::apply(image, xKernel, type);

            Kernel yKernel = Kernel::createGaussSeparateKernelY(sigma);
            FImage resultY = Convolution::apply(resultX, yKernel, type);
            resultY.normalize();
            ui->label->setPixmap(QPixmap::fromImage(resultY.toQImage()));
        } else {
            Kernel gaussKernel = Kernel::createGaussKernel(sigma);
            FImage result = Convolution::apply(image, gaussKernel, type);
            result.normalize();
            ui->label->setPixmap(QPixmap::fromImage(result.toQImage()));
        }
    }
}

void MainWindow::on_bOctaves_clicked() {
    if(original != nullImage){
        double sigma = ui->dsStartSigma->value();
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        FImage image(original);
        int octavesCount = ui->spOctavesCount->value();
        int levelPerOctave = ui->spLevelPerOctave->value();
        PyramidBuilder builder(octavesCount, levelPerOctave, sigma);
        builder.createOctaves(image, type);
    }
}

void MainWindow::on_bMoravec_clicked() {
    if(original != nullImage){
        EdgeType type = (EdgeType)ui->cbEdgeTypes->currentIndex();
        float threshold = (float)ui->sliderThreshold->value() / 1000.;
        FImage image(original);
        InterestPointsDetector detector(image, type);
        if(ui->chbFilter->isChecked()){
            detector.enableFilter(ui->spinInterestPointsCount->value());
        }
        QVector<InterestPoint*> points = detector.detectMoravec(threshold);
        QImage copy(original);
        foreach (InterestPoint* point, points) {
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
        ui->label->setPixmap(QPixmap::fromImage(copy));
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
        QVector<InterestPoint*> points = detector.detectHarris(threshold);
        QImage copy(original);

        foreach (InterestPoint* point, points) {
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
        ui->label->setPixmap(QPixmap::fromImage(copy));
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
        ui->label->setPixmap(QPixmap::fromImage(*result));
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
        QVector<InterestPoint*> points1 = detector1.detectHarris(threshold);
        QVector<InterestPoint*> points2 = detector2.detectHarris(threshold);
        QVector<Descriptor*> descriptors1 = DescriptorSearcher::search(image1,
                    points1, type, ui->chbGaussSmooth->isChecked(), sigma);

        QVector<Descriptor*> descriptors2 = DescriptorSearcher::search(image2,
                    points2, type, ui->chbGaussSmooth->isChecked(), sigma);

        QVector<QPair<Descriptor*, Descriptor*>*> pairs =
                DescriptorSearcher::associate(descriptors1, descriptors2);
        int height = original.height() > img2.height()
                     ? original.height() : img2.height();
        QImage* result = new QImage(original.width() + img2.width(), height,
                      QImage::Format_ARGB32);
        QPainter painter(result);
        painter.drawImage(0, 0, original);
        painter.drawImage(original.width(), 0, img2);        
        for (int i = 0; i < pairs.size(); ++i) {
            QColor color = QColor((5 * i) % 256,
                                  (10 * i) % 256,
                                  (128 + 22 * i) % 256);
            painter.setPen(color);
            QPair<Descriptor*, Descriptor*>* pair = pairs.at(i);
            Descriptor* d1 = pair->first;
            Descriptor* d2 = pair->second;
            painter.drawRect(d1->getX(), d1->getY(), 3, 3);
            painter.drawRect(d2->getX() + img2.width(), d2->getY(), 3, 3);
            painter.drawLine(d1->getX(), d1->getY(),
                             d2->getX() + img2.width(), d2->getY());
        }
        ui->label->setPixmap(QPixmap::fromImage(*result));
    }
}
