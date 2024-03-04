#ifndef QR_DETECTION_H
#define QR_DETECTION_H

#include <QObject>
#include <QWidget>
#include "zbar.h"
#include "opencv2/opencv.hpp"
#include <string.h>
#include <stdio.h>
#include <QElapsedTimer>
#include <senddataudp.h>

using namespace std;
using namespace cv;
using namespace zbar;


class QR_detection : public QObject
{
    Q_OBJECT
public:
    explicit QR_detection(QObject *parent = nullptr);

    typedef  struct{
        string type;
        string data;
        vector <Point> QRlocation;
    }detectQRobject;

    QElapsedTimer t3;

    string Zbar_type_goal = "QR-Code";
    string Zbar_data_goal = "Landing";

    Mat gray_QR;
    char QRdataReady = 0;
    sendDataUDP sender6;

    void detectQR(Mat &QRimg, vector<detectQRobject> &QRdetection);
    void DisplarQR(Mat &QRimg, vector<detectQRobject> &QRdetection);

signals:
    void sendingFlag(char flag);

public slots:
    void QR_result(Mat QRimg);
};

#endif // QR_DETECTION_H
