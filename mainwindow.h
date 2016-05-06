#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagehandler.h"
#include "pyramid.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_bLoadImage_clicked();
        void on_bSobelX_clicked();
        void on_bSobelY_clicked();

        void on_bSobelXY_clicked();
        void on_sliderRadius_valueChanged(int value);

        void on_bGauss_clicked();

        void on_bOctaves_clicked();

private:
        Ui::MainWindow *ui;
        ImageHandler* handler = NULL;
};

#endif // MAINWINDOW_H
