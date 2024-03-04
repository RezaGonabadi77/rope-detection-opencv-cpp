#ifndef SENDDATAUDP_H
#define SENDDATAUDP_H
#include <QUdpSocket>
#include <QObject>
#include <QDebug>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class sendDataUDP : public QObject
{
    Q_OBJECT
public:
    explicit sendDataUDP(QObject *parent = nullptr);
    QHostAddress *recieve_ip;


signals:

public slots:
    void sendSetPointData(cv::Point,int,char);
};

#endif // SENDDATAUDP_H
