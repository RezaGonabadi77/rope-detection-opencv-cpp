#include "capture.h"


Capture::Capture(string camNum, QObject *parent) : QObject(parent)
{

    //cap.open(camNum);
    cap.open("http://admin:@192.168.1.40/videostream.asf?user=admin&pwd=&resolution=8&rate=0");
    namedWindow("original frame",WINDOW_NORMAL);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
    cap.set(CV_CAP_PROP_FPS,FRAME_FPS);

    if( cap.isOpened() )
    {
        cout<<"cam" <<camNum<< "opened .";
        cap >> frame;
        //IP_SAVED.write(frame);

        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(timeOut()));
        timer->start(2);

    }
    else
    {
        cout << "Couldn't open the camera, exit.\n";
        exit(0);
    }
}

void Capture::timeOut()
{
    cap >> frame;
    //qDebug()<<"CAM Thread:"<<QThread::currentThreadId();
    emit imageReady_line(frame);
}
