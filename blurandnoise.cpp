#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>
#include "CVClass.h"

using namespace cv;
using namespace std;

typedef unsigned char byte;
//int main(){

//    Mat src = imread("1.jpg", 0);//以灰度形式读取图片
//    Mat dst;
//    gradientGray(src, dst);
//    imwrite("2.jpg",dst); //保存文件，梯度图
//    imshow("src", src);
//    imshow("dst", dst);
//    waitKey(0);

//}

float CVClass::gradientGray(Mat &src, Mat &mag)
{
    const int H = src.rows, W = src.cols;
    Mat Ix(H, W, CV_32S), Iy(H, W, CV_32S);
    //因为计算出的梯度值可能有正有负，且值也可能会很大，故数据类型为整形
    unsigned long Grow[src.rows];
    unsigned long Gmean=0;
    // 求水平方向梯度，处理左右边缘像素
    for (int y = 0; y < H; y++){
        Ix.at<int>(y, 0) = abs(src.at<byte>(y, 1) - src.at<byte>(y, 0)) * 2;
        for (int x = 1; x < W - 1; x++)
            Ix.at<int>(y, x) = abs(src.at<byte>(y, x + 1) - src.at<byte>(y, x - 1));
        Ix.at<int>(y, W - 1) = abs(src.at<byte>(y, W - 1) - src.at<byte>(y, W - 2))*2;
    }
    // 求垂直方向梯度，处理左右边缘像素
    for (int x = 0; x < W; x++)  {
        Iy.at<int>(0, x) = abs(src.at<byte>(1, x) - src.at<byte>(0, x)) * 2;
        for (int y = 1; y < H - 1; y++)
            Iy.at<int>(y, x) = abs(src.at<byte>(y + 1, x) - src.at<byte>(y - 1, x));
        Iy.at<int>(H - 1, x) = abs(src.at<byte>(H - 1, x) - src.at<byte>(H - 2, x)) * 2;
    }
    /*for (int j = 0; j < H; j++)
        for (int k = 0; k < W; k++)
        {
            mag.at<byte>(j, k) = min(Ix.at<int>(j,k) + Iy.at<int>(j, k), 255);
        }*/
    convertScaleAbs(min(Ix + Iy, 255), mag); //这句话和上面的for循环是同样的功能
    Mat tmp;
    cvtColor(mag,tmp,COLOR_GRAY2RGB);
    emit pushWin2(tmp);
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            Gmean+=mag.at<byte>(i,j);
//            cout<<"test"<<(int)mag.at<byte>(i,j)<<endl;
        }
    }
    Gmean/=(src.rows*src.cols);
//    cout<<"Gmean"<<Gmean<<endl;
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            if(mag.at<byte>(i,j)<(Gmean)){
                mag.at<byte>(i,j)=0;
            }
//            cout<<"test"<<(int)mag.at<byte>(i,j)<<endl;
        }
    }

    long BlurCnt=1;
    long SumBlur=0;
    float Tbr=0.5;
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            if(mag.at<byte>(i,j)>0){
                BlurCnt+=1;
                int A=(mag.at<byte>(i-1,j)+mag.at<byte>(i+1,j)+mag.at<byte>(i,j-1)+mag.at<byte>(i,j+1))/4;
                float BR=fabs(mag.at<byte>(i,j)-A)/(float)A;
                if(BR>Tbr){
//                    cout<<"check"<<endl;
                    SumBlur+=1;
                }
            }
//            cout<<"test"<<(int)mag.at<byte>(i,j)<<endl;
        }
    }
//    cout<<SumBlur<<" "<<BlurCnt<<endl;
//    cout<<"ratio"<<((float)SumBlur/BlurCnt)<<endl;
//    if(SumBlur>100&&BlurCnt>100){
     emit showBlurParam(((float)SumBlur/BlurCnt));
//        }

}
