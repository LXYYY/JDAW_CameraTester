#include "ctmainwindow.h"
#include "ui_ctmainwindow.h"
#include "CVClass.h"
#include <iostream>
#include <QThread>
#include <QMetaType>
#include <QMessageBox>
CVClass* cvClass=new CVClass();
using namespace std;
CTMainWindow::CTMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CTMainWindow)
{
    qRegisterMetaType<Mat>("Mat");
    qRegisterMetaType<string>("string");
    ui->setupUi(this);

    initUI();

    connect(cvClass,SIGNAL(pushWin1(Mat)),this,SLOT(pushWin1(Mat)));
    connect(cvClass,SIGNAL(pushWin2(Mat)),this,SLOT(pushWin2(Mat)));
    connect(this,SIGNAL(takeAPicture()),cvClass,SLOT(takeAPicture()));
    connect(this,SIGNAL(startCalc()),cvClass,SLOT(startCalc()));
    connect(this,SIGNAL(startCapture()),cvClass,SLOT(startCapture()));
    connect(this,SIGNAL(startChessboard()),cvClass,SLOT(startChessboard()));
    connect(this,SIGNAL(startBlurCheck()),cvClass,SLOT(startBlurCheck()));
    connect(cvClass,SIGNAL(showBlurParam(float,float)),this,SLOT(showBlurParam(float,float)));
    connect(cvClass,SIGNAL(calibrateFailWarn()),this,SLOT(calibrateFailWarn()));
    connect(cvClass,SIGNAL(stateMonitor(int,int)),this,SLOT(stateMonitor(int,int)));
    connect(this,SIGNAL(setCameraParam(int,double)),cvClass,SLOT(setCameraParam(int,double)));
    connect(this,SIGNAL(getCameraParam(int)),cvClass,SLOT(getCameraParam(int)));
    connect(cvClass,SIGNAL(showCameraParam(double)),this,SLOT(showCameraParam(double)));
    connect(this,SIGNAL(setCameraName(string)),cvClass,SLOT(setCameraName(string)));
    connect(this,SIGNAL(setCameraID(int)),cvClass,SLOT(setCameraID(int)));
    connect(cvClass,SIGNAL(showIntrinsicParam(Mat,Mat)),this,SLOT(showIntrinsicParam(Mat,Mat)));
}

CTMainWindow::~CTMainWindow()
{
    delete ui;
}

void CTMainWindow::initUI()
{
    ui->paramComboBox->addItem("CAP_PROP_SATURATION");
    ui->paramComboBox->addItem("CAP_PROP_CONTRAST");
    ui->paramComboBox->addItem("CAP_PROP_EXPOSURE");
    ui->paramComboBox->addItem("CAP_PROP_WHITE_BALANCE_BLUE_U");
    ui->paramComboBox->addItem("CAP_PROP_WHITE_BALANCE_RED_V");

    ui->ParamSpinBox->setRange(-10000,10000);

    ui->cameraIDSpinBox->setValue(2);
}

void CTMainWindow::on_OpenCameraButton_clicked()
{
    emit startCapture();
    emit setCameraID(ui->cameraIDSpinBox->value());
    if(cvClass->isRunning()==false)
        cvClass->start();
    else cvClass->quit();
}

void CTMainWindow::pushWin1(Mat img)
{
    QImg1=QImage(img.data,img.cols,img.rows,img.cols*img.channels(),QImage::Format_RGB888);
    ui->Win1->clear();
    ui->Win1->setPixmap(QPixmap::fromImage(QImg1));
    ui->Win1->repaint();
}

void CTMainWindow::pushWin2(Mat img)
{
    QImg2=QImage(img.data,img.cols,img.rows,img.cols*img.channels(),QImage::Format_RGB888);
    ui->Win2->clear();
    ui->Win2->setPixmap(QPixmap::fromImage(QImg2));
    ui->Win2->repaint();
}

void CTMainWindow::on_ShootingButton_clicked()
{
    emit takeAPicture();
}

void CTMainWindow::on_CalcButton_clicked()
{
    emit startCalc();
}

void CTMainWindow::on_ChessboardButton_clicked()
{
    startChessboard();
}

void CTMainWindow::on_BlurCheckButton_clicked()
{
    emit startBlurCheck();
}

void CTMainWindow::showBlurParam(float br,float noise)
{
    char text[50];
    sprintf(text,"BlurParam:%-3.2f , Noise:%-3.2f",br,noise);

    ui->BlurParamEdit->setText(text);
    //    ui->BlurParamEdit->repaint();

}

void CTMainWindow::calibrateFailWarn()
{
QMessageBox::warning(
            NULL, "warning", "Content",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
}

void CTMainWindow::stateMonitor(int id, int state)
{
    switch(id){
    case captureID:
        if(state==0)
            ui->OpenCameraButton->setStyleSheet(
                        "background-color: rgb(255,255,255)");
        else
            ui->OpenCameraButton->setStyleSheet(
                    "background-color: rgb(0,255,0)");
        break;
    case chessboardID:
        if(state==0)
            ui->ChessboardButton->setStyleSheet(
                        "background-color: rgb(255,255,255)");
        else
            ui->ChessboardButton->setStyleSheet(
                    "background-color: rgb(0,255,0)");
        break;
    case blurCheckID:
        if(state==0)
            ui->BlurCheckButton->setStyleSheet(
                        "background-color: rgb(255,255,255)");
        else
            ui->BlurCheckButton->setStyleSheet(
                    "background-color: rgb(0,255,0)");
        break;
    }
}

void CTMainWindow::on_setParamButton_clicked()
{
    QString text;
    int propId;
    double value;

    text=ui->paramComboBox->currentText();
    if(text=="CAP_PROP_SATURATION") propId=CAP_PROP_SATURATION;
    if(text=="CAP_PROP_CONTRAST") propId=CAP_PROP_CONTRAST;
    if(text=="CAP_PROP_EXPOSURE") propId=CAP_PROP_EXPOSURE;
    if(text=="CAP_PROP_WHITE_BALANCE_BLUE_U") propId=CAP_PROP_WHITE_BALANCE_BLUE_U;
    if(text=="CAP_PROP_WHITE_BALANCE_RED_V") propId=CAP_PROP_WHITE_BALANCE_RED_V;

    value=ui->ParamSpinBox->value();

    emit setCameraParam(propId,value);
}

void CTMainWindow::on_paramComboBox_currentIndexChanged(const QString &arg1)
{
    QString text;
    text=ui->paramComboBox->currentText();
    int propId;

    if(text=="CAP_PROP_SATURATION") propId=CAP_PROP_SATURATION;
    if(text=="CAP_PROP_CONTRAST") propId=CAP_PROP_CONTRAST;
    if(text=="CAP_PROP_EXPOSURE") propId=CAP_PROP_EXPOSURE;
    if(text=="CAP_PROP_WHITE_BALANCE_BLUE_U") propId=CAP_PROP_WHITE_BALANCE_BLUE_U;
    if(text=="CAP_PROP_WHITE_BALANCE_RED_V") propId=CAP_PROP_WHITE_BALANCE_RED_V;

    emit getCameraParam(propId);
}

void CTMainWindow::showCameraParam(double value)
{
    ui->ParamSpinBox->setValue(value);
}

void CTMainWindow::on_cameraIDEdit_returnPressed()
{
    emit setCameraName(ui->cameraIDEdit->text().toStdString());
}

void CTMainWindow::showIntrinsicParam(Mat cameraMatrix,Mat distCoeffs)
{
    char text[200];
    sprintf(text,"fx:%7f,fy:%7f\ncx:%7f,cy:%7f\nk1:%7f,k2:%7f",
           cameraMatrix.at<double>(0,0),cameraMatrix.at<double>(1,1),
           cameraMatrix.at<double>(0,2),cameraMatrix.at<double>(1,2),
           distCoeffs.at<double>(0,0),distCoeffs.at<double>(0,1));

    ui->intrinsicParamEdit->setText(text);
    ui->intrinsicParamEdit->repaint();
}
