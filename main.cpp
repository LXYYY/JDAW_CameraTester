#include "ctmainwindow.h"
#include <QApplication>
#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CTMainWindow w;
    w.show();

    return a.exec();
}
