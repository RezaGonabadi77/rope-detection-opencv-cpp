#include "qr_detection.h"

QR_detection::QR_detection(QObject *parent) : QObject(parent)
{
    namedWindow("QRResults",WINDOW_NORMAL);

}

void QR_detection::QR_result(Mat QRimg)
{
    //t3.start();
    if(QRdataReady == 0){
        //imshow("QRResults",QRimg);

        vector<detectQRobject> QR_detection;

        detectQR(QRimg,QR_detection);

        DisplarQR(QRimg,QR_detection);
    }

    //cout << " QR_delay" << t3.elapsed() << endl;

}

void QR_detection::detectQR(Mat &QRimg, vector<QR_detection::detectQRobject> &QRdetection)
{

    ImageScanner scanQR;

    scanQR.set_config(ZBAR_NONE,ZBAR_CFG_ENABLE,1);

    cvtColor(QRimg,gray_QR,CV_BGR2GRAY);

    Image image(QRimg.cols,QRimg.rows,"Y800",(uchar *)gray_QR.data , QRimg.cols*QRimg.rows);

    int n = scanQR.scan(image);

    for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
      {
        detectQRobject obj;

        obj.type = symbol->get_type_name();
        obj.data = symbol->get_data();

        // Print type and data
        cout << "Type : " << obj.type << endl;
        cout << "Data : " << obj.data << endl << endl;

        //for control servo motor of IP cam
        if(obj.type == Zbar_type_goal && obj.data == Zbar_data_goal) {
            QRdataReady = 1;
            emit sendingFlag(QRdataReady);
            sender6.sendSetPointData(Point(0,0),0,QRdataReady);
        }
        // Obtain location
        for(int i = 0; i< symbol->get_location_size(); i++)
        {
          obj.QRlocation.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
        }

        QRdetection.push_back(obj);
      }

}

void QR_detection::DisplarQR(Mat &QRimg, vector<QR_detection::detectQRobject> &QRdetection)
{
    for(int i = 0; i < QRdetection.size(); i++)
      {
        vector<Point> points = QRdetection[i].QRlocation;
        vector<Point> hull;

        // If the points do not form a quad, find convex hull
        if(points.size() > 4)
          convexHull(points, hull);
        else
          hull = points;

        // Number of points in the convex hull
        int n = hull.size();

        for(int j = 0; j < n; j++)
        {
          line(QRimg, hull[j], hull[ (j+1) % n], Scalar(255,0,0), 3);
        }

      }

      // Display results
      imshow("QRResults", QRimg);

}
