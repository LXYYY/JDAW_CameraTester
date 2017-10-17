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
    for(int i=1;i<src.rows-1;i++){
        for(int j=1;j<src.cols-1;j++){
            if(mag.at<byte>(i,j)>0){
                BlurCnt+=1;
                int A=(mag.at<byte>(i-1,j)+mag.at<byte>(i+1,j)+mag.at<byte>(i,j-1)+mag.at<byte>(i,j+1))/4;
                float BR;
                if(A>0)
                    BR=fabs(mag.at<byte>(i,j)-A)/(float)A;
                else BR=0;
                if(BR>Tbr){
                    SumBlur+=1;
                }
            }
        }
    }


    /////////////noise check////////////
    Mat srcNoise;
    src.copyTo(srcNoise);
    for(int i=1;i<srcNoise.rows-1;i++){
        for(int j=1;j<srcNoise.cols-1;j++){
            int tmp=0;
            for(int m=-1;m<1;m++)
                for(int n=-1;n<1;n++){
                    tmp+=srcNoise.at<byte>(i+m,j+n);
                }
            tmp/=9;
            srcNoise.at<byte>(i,j)=tmp;
        }
    }

    for (int y = 0; y < H; y++){
        Ix.at<int>(y, 0) = abs(srcNoise.at<byte>(y, 1) - srcNoise.at<byte>(y, 0)) * 2;
        for (int x = 1; x < W - 1; x++)
            Ix.at<int>(y, x) = abs(srcNoise.at<byte>(y, x + 1) - srcNoise.at<byte>(y, x - 1));
        Ix.at<int>(y, W - 1) = abs(srcNoise.at<byte>(y, W - 1) - srcNoise.at<byte>(y, W - 2))*2;
    }
    // 求垂直方向梯度，处理左右边缘像素
    for (int x = 0; x < W; x++)  {
        Iy.at<int>(0, x) = abs(srcNoise.at<byte>(1, x) - srcNoise.at<byte>(0, x)) * 2;
        for (int y = 1; y < H - 1; y++)
            Iy.at<int>(y, x) = abs(srcNoise.at<byte>(y + 1, x) - srcNoise.at<byte>(y - 1, x));
        Iy.at<int>(H - 1, x) = abs(srcNoise.at<byte>(H - 1, x) - srcNoise.at<byte>(H - 2, x)) * 2;
    }

    convertScaleAbs(min(Ix + Iy, 255), mag); //这句话和上面的for循环是同样的功能

    Gmean=0;
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
            if(mag.at<byte>(i,j)>(Gmean)){
                mag.at<byte>(i,j)=0;
            }
//            cout<<"test"<<(int)mag.at<byte>(i,j)<<endl;
        }
    }

    Gmean=0;
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            Gmean+=mag.at<byte>(i,j);
//            cout<<"test"<<(int)mag.at<byte>(i,j)<<endl;
        }
    }
    Gmean/=(src.rows*src.cols);

    long NoiseCnt=1;
    long SumNoise=0;
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            if(mag.at<byte>(i,j)>(Gmean)){
                SumNoise++;
            }
            NoiseCnt++;
//            cout<<"test"<<(int)mag.at<byte>(i,j)<<endl;
        }
    }

    emit showBlurParam(((float)SumBlur/BlurCnt),((float)SumNoise/NoiseCnt));
//    emit showBlurParam(((float)SumBlur/BlurCnt),0);
//    cout<<(float)SumBlur/BlurCnt<<endl;
//        }

}
