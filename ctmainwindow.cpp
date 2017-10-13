#include "ctmainwindow.h"
#include "ui_ctmainwindow.h"
#include "CVClass.h"
#include <iostream>
#include <QThread>
#include <QMetaType>
CVClass* cvClass=new CVClass();

CTMainWindow::CTMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CTMainWindow)
{
    qRegisterMetaType<Mat>("Mat");
    ui->setupUi(this);

    connect(cvClass,SIGNAL(pushWin1(Mat)),this,SLOT(pushWin1(Mat)));
    connect(cvClass,SIGNAL(pushWin2(Mat)),this,SLOT(pushWin2(Mat)));
    connect(this,SIGNAL(takeAPicture()),cvClass,SLOT(takeAPicture()));
    connect(this,SIGNAL(startCalc()),cvClass,SLOT(startCalc()));
    connect(this,SIGNAL(startCapture()),cvClass,SLOT(startCapture()));
    connect(this,SIGNAL(startChessboard()),cvClass,SLOT(startChessboard()));
    connect(this,SIGNAL(startBlurCheck()),cvClass,SLOT(startBlurCheck()));
    connect(cvClass,SIGNAL(showBlurParam(float)),this,SLOT(showBlurParam(float)));
}

CTMainWindow::~CTMainWindow()
{
    delete ui;
}

void CTMainWindow::on_OpenCameraButton_clicked()
{
    emit startCapture();
    if(cvClass->isRunning()==false)
        cvClass->start();
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

void CTMainWindow::showBlurParam(float br)
{
    char text[50];
    sprintf(text,"BlurParam:%-3.2f",br);

    ui->BlurParamEdit->setText(text);
    ui->BlurParamEdit->repaint();
}
