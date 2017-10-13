#ifndef CTMAINWINDOW_H
#define CTMAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
using namespace cv;

namespace Ui {
class CTMainWindow;
}

class CTMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CTMainWindow(QWidget *parent = 0);
    ~CTMainWindow();

    QImage QImg1,QImg2;

private slots:
    void on_OpenCameraButton_clicked();
    void pushWin1(Mat img);
    void pushWin2(Mat img);

    void on_ShootingButton_clicked();

    void on_CalcButton_clicked();

signals:
    void takeAPicture(void);
    void startCalc(void);
    void startCapture(void);

private:
    Ui::CTMainWindow *ui;
};

#endif // CTMAINWINDOW_H
