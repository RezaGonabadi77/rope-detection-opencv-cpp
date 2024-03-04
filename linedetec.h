#ifndef LINEDETEC_H
#define LINEDETEC_H

#include <QObject>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QDebug>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include "colordetection.h"
#include <QElapsedTimer>
#include "senddataudp.h"
#include "qr_detection.h"

#define Canny_maxThreshold      100
#define canny_maxKernelSize     9
#define MAX_RHO                 10
#define PI                      3.14159
//#define MinArea                 1500


using namespace cv;
using namespace std;

class LineDetec : public QObject
{
    Q_OBJECT
public:
    explicit LineDetec(QObject *parent = nullptr);
    float DistanceTwoPoint(Point A,Point B);
    float Calculate_Angle(Point A,Point B);
    void BubbleSort(float array[],int n);
    void bubbleSort_point_x(Point arr[], int n);
    void bubbleSort_point_y(Point arr[], int n);
    void emit_data(int Flag,Mat image);
    Mat edges;
    int Canny_lowThreshold = 0;
    int kernel_size = 5;
    int raito = 3;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Vec2f> lines;

    double contours_area;
    Point prev_mc;
    Rect poly;
    int white_count;

    int X_distance;
    int Y_distance;
    float distance;
    float Dist;
    float Dist_Sum=0;
    float Prev_distance;
    int Angle=0;
    int Prev_Angle;
    int White_Count=0;
    //debug
    int counter_if_1 = 0;
    int counter_if_2 = 0;
    int counter_if_3 = 0;
    int counter_if_4 = 0;
    int counter_if_5 = 0;
    //
    int MinArea = 2000;
    int MinArea_Circle = 3000;

    Point destination;
    Point Prev_destination;
    int data_Ready=0;
    QElapsedTimer t2;
    sendDataUDP sender5;
    QR_detection QR_line_object;

    char QRdataReady=0;

signals:
    void findLine(Point,int);

public slots:
    void get_image_Line(Mat frame);
    void get_QR_flag(char flag);
};


#endif // LINEDETEC_H
