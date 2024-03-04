#ifndef COLORDETECTION_H
#define COLORDETECTION_H

#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QThread>
#include <QObject>
#include <QElapsedTimer>
#include <QDebug>

#define EROSION_ELEM    1
#define EROSRION_SIZE   1   //3
#define DILATION_ELEM   1
#define DILATION_SIZE   6   //5
#define max_elem        2
#define max_kernel_size 10

#define MAX_HSV 255


using namespace cv;
using namespace std;

class colorDetection : public QObject
{
    Q_OBJECT
public:

    explicit colorDetection(QObject *parent = nullptr);
    Mat detectBlueColorInrange(Mat BP_image);
    Mat Hist_backproj(Mat hue);
    void do_Hist_And_Backproj(void);
    Mat backprojectionResult(Mat src);
    void Erosion_Dilation(void);
    void Remove_green_battery(Mat src);

    Mat Blue;
    Mat green;
    Mat inrange_color;
    Mat hsv;
    Mat black;
    Mat hue;
    Mat final_inRange,final_Hist,final_Backproj;
    Mat final_detect_color;
    Mat backproj;
    Mat histimg;
    int hsize=64;
    int medianblur_filter_kernel1 = 11;
    int medianblur_filter_kernel2 = 9;
    int medianblur_filter_kernel3 = 9;
    int vmin = 10, vmax = 256, smin = 50;
    //blue
    int low_hue_blue=80;
    int high_hue_blue=130;
    int low_sat_blue=20;
    int high_sat_blue=255;
    int low_value_blue=180;         ///ps3 200
    int high_value_blue=255;
    //green
    int low_hue_green=50;
    int high_hue_green=90;
    int low_sat_green=15;
    int high_sat_green=160;
    int low_value_green=50;
    int high_value_green=200;
    //pink
    int low_hue_pink=150;
    int high_hue_pink=180;
    int low_sat_pink=55;
    int high_sat_pink=130;
    int low_value_pink=110;
    int high_value_pink=255;

    float hranges[2];
    const float* phranges;
    Mat hist;
    Mat sub,mask;

    //erode and dilate
    Mat ERODE_elem, DILATE_elem;
    int erosion_elem = EROSION_ELEM;
    int erosion_size = EROSRION_SIZE;
    int dilation_elem = DILATION_ELEM;
    int dilation_size = DILATION_SIZE;
    //
    int green_detect_flag = 1;
    int blue_detect_flag = 0;
    int pink_detect_flag = 0;
    QElapsedTimer t1;

signals:
    void colorDetectReady(Mat b);

public slots:
    void getColorDetection(Mat a);
    void get_QR_flag_color(char flag);
};

#endif // COLORDETECTION_H
