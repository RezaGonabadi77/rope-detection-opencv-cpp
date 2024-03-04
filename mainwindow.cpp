#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


//https://doc.qt.io/qt-5/thread-basics.html
//https://doc.qt.io/qt-5/signalsandslots.html

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cam=new Capture("/home/reza_gonabadi/vision/build-q3-Desktop-Debug/outcpp3.avi");
    cam_thread = new QThread();
    cam -> moveToThread(cam_thread);

    linedetec = new LineDetec();
    linedetec_thread = new QThread();
    linedetec -> moveToThread(linedetec_thread);

    Colordetect = new colorDetection();
    colordetect_thread = new QThread();
    Colordetect -> moveToThread(colordetect_thread);

    send = new sendDataUDP();
    send_thread = new QThread();
    send -> moveToThread(send_thread);


    QRobject = new QR_detection();


    connect(cam,SIGNAL(imageReady_line(Mat)),QRobject,SLOT(QR_result(Mat)),Qt::DirectConnection);
    connect(cam,SIGNAL(imageReady_line(Mat)),Colordetect,SLOT(getColorDetection(Mat)),Qt::DirectConnection);
    connect(Colordetect,SIGNAL(colorDetectReady(Mat)),linedetec,SLOT(get_image_Line(Mat)),Qt::DirectConnection);
    connect(linedetec,SIGNAL(findLine(Point,int)),send,SLOT(sendSetPointData(cv::Point,int)),Qt::DirectConnection);
    connect(QRobject,SIGNAL(sendingFlag(char)),Colordetect,SLOT(get_QR_flag_color(char)),Qt::DirectConnection);
    connect(QRobject,SIGNAL(sendingFlag(char)),linedetec,SLOT(get_QR_flag(char)),Qt::DirectConnection);


    cam_thread->start();
    linedetec_thread->start();
    colordetect_thread->thread();
    send_thread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_smoothing(int b) //note: you can pass Mat as one of this slot arguments and use that instead of using cam->frame.
{
    cout<<b<<endl;
    cout<<"in slot do something"<<endl;
    namedWindow("frame",WINDOW_NORMAL);
    Mat new_frame=cam->frame.clone();
    medianBlur(new_frame,new_frame,5);
    imshow("frame",new_frame);
    //waitKey(2);
}


