#ifndef CTMAINWINDOW_H
#define CTMAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#define captureID 0
#define chessboardID 1
#define blurCheckID 2

using namespace cv;
using namespace std;

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
    void initUI(void);
private slots:
    void on_OpenCameraButton_clicked();
    void pushWin1(Mat img);
    void pushWin2(Mat img);

    void on_ShootingButton_clicked();

    void on_CalcButton_clicked();

    void on_ChessboardButton_clicked();

    void on_BlurCheckButton_clicked();

    void showBlurParam(float br,float noise);
    void calibrateFailWarn(void);
    void stateMonitor(int id,int state);
    void on_setParamButton_clicked();

    void on_paramComboBox_currentIndexChanged(const QString &arg1);
    void showCameraParam(double value);
    void on_cameraIDEdit_returnPressed();
    void showIntrinsicParam(Mat cameraMatrix, Mat distCoeffs);
signals:
    void takeAPicture(void);
    void startCalc(void);
    void startCapture(void);
    void startChessboard(void);
    void startBlurCheck(void);
    void setCameraParam(int propId,double value);
    void getCameraParam(int propId);
    void setCameraName(string);
    void setCameraID(int);
private:
    Ui::CTMainWindow *ui;
};

#endif // CTMAINWINDOW_H
