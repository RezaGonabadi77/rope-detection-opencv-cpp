#include "linedetec.h"
LineDetec::LineDetec(QObject *parent) : QObject(parent)
{
    namedWindow("line",WINDOW_NORMAL);
    createTrackbar("contour size","line",&MinArea,4000);

}


void LineDetec::get_image_Line(Mat frame)
{
    //t2.start();
    if(QRdataReady == 0){

    //    qDebug()<<"Line Thread:"<<QThread::currentThreadId();

        //find contours function

        findContours(frame,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE);
        vector < vector< Point >> contours_poly(contours.size());
        //Gray To BGR
        cvtColor(frame,frame,CV_GRAY2BGR);

        //drawContours(frame,contours,-1,Scalar(0,255,0),3);

        //cout << "    " <<contours.size() << endl;

        for (unsigned int i =0 ; i<contours.size() ; i++ ) {

            approxPolyDP(contours[i],contours_poly[i],3,true);

        }

        /// Get the moments
        vector <Moments>  mu(contours_poly.size() );
        for(unsigned int i = 0; i < contours_poly.size(); i++ ){
            mu[i] = moments( contours_poly[i], false );
        }
        ///  Get the mass centers:
        vector<Point2f> mc( contours_poly.size() );
        for(unsigned int i = 0; i < contours_poly.size(); i++ ){
            contours_area = contourArea(contours_poly[i]);
            if (contours_area > MinArea) {
                mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
                circle(frame,mc[i],10,Scalar(0,255,0),1,8,0);
            }
        }

        float near_mc[contours_poly.size()];
        for (unsigned int i = 0; i < contours_poly.size() ; i++ ) {
            near_mc[i] = DistanceTwoPoint(mc[i],prev_mc);
        }

        BubbleSort(near_mc,contours_poly.size());



        for (unsigned int i = 0; i < contours_poly.size() ; i++) {

    //        poly = boundingRect(contours_poly[i]);

    //        white_count = countNonZero(frame(poly));


            destination = mc[i];

            contours_area = contourArea(contours_poly[i]);

            if(contours_area > MinArea /*&& DistanceTwoPoint(mc[i],prev_mc) == near_mc[0*/){

                data_Ready = 1;

                for (unsigned int w = 0; w < contours_poly[i].size() ; w++) {

                    /////////////
                    if(contours_poly[i][w].x == frame.cols-1 && contours_poly[i][w].y < mc[i].y){

                        destination = contours_poly[i][w];
                        Angle=-int((PI/2+atan((mc[i].y-destination.y)/(mc[i].x-destination.x)))*180/PI);
                        counter_if_1++;
                        break;

                    }
                    ////////////////
                    else if (contours_poly[i][w].x == 0 && contours_poly[i][w].y < mc[i].y) {

                        Point sorted[contours_poly[i].size()];
                        for(unsigned int a=0;a<contours_poly[i].size();a++)
                            sorted[a]=contours_poly[i][a];
                        bubbleSort_point_x(sorted,contours_poly[i].size());
                        destination=(sorted[0]+sorted[1])*0.5;
                        Angle=int((PI/2-atan((mc[i].y-destination.y)/(mc[i].x-destination.x)))*180/PI);
                        counter_if_2++;
                        break;
                    }///////////////
                    else if (contours_poly[i][w].x < mc[i].x && contours_poly[i][w].y <= 10) {
                        //LEFT
                        Point sorted[contours_poly[i].size()];
                        for(unsigned int a=0;a<contours_poly[i].size();a++)
                            sorted[a]=contours_poly[i][a];
                        bubbleSort_point_y(sorted,contours_poly[i].size());
                        destination=(sorted[0]+sorted[1])*0.5;
                       //   circle(sub,p2,8,color,-1);
                        float tetha=(PI/2-atan((mc[i].y-destination.y)/(mc[i].x-destination.x)))*180/PI;
                        counter_if_3++;
                        if(abs(tetha-180)>10)
                        {
                            if(destination.x<mc[i].x)
                            {
                                Angle=int((PI/2-atan((mc[i].y-destination.y)/(mc[i].x-destination.x)))*180/PI);
                            }
                        }
                        else
                        {
                            //qDebug()<< "straight angle " <<0;
                            Angle=0;
                        }

                        break;

                    }
                    //////////////////////////
                    else if (contours_poly[i][w].x > mc[i].x && contours_poly[i][w].y <= 10) {
                        //RIGHT
                        Point sorted[contours_poly[i].size()];
                        for(unsigned int a=0;a<contours_poly[i].size();a++)
                            sorted[a]=contours_poly[i][a];
                        bubbleSort_point_y(sorted,contours_poly[i].size());
                        destination=(sorted[0]+sorted[1])*0.5;

                        Angle=-int((PI/2+atan((mc[i].y-destination.y)/(mc[i].x-destination.x)))*180/PI);
                        counter_if_4++;
                    }
                    /////////////////////
                    else {

                        Point sorted[contours_poly[i].size()];
                        for(unsigned int a=0;a<contours_poly[i].size();a++)
                            sorted[a]=contours_poly[i][a];
                        bubbleSort_point_y(sorted,contours_poly[i].size());
                        destination=(sorted[0]+sorted[1])*0.5;
                        counter_if_5++;
                        for(uint a=0;a<contours_poly[i].size();a++)
                        {
                            if(sorted[a].x==frame.cols-1 || sorted[a].x== 0 )
                                destination=sorted[a];
                        }

                        if(destination.x<mc[i].x)
                        {
                            Point sorted[contours_poly[i].size()];
                            for(unsigned int a=0;a<contours_poly[i].size();a++)
                                sorted[a]=contours_poly[i][a];
                            bubbleSort_point_x(sorted,contours_poly[i].size());
                            destination=(sorted[0]+sorted[1])*0.5;

                            Angle=int((PI/2-atan((mc[i].y-destination.y)/(mc[i].x-destination.x)))*180/PI);
                        }
                        else
                        {
                            Angle=-int((PI/2+atan((mc[i].y-destination.y)/(mc[i].x-destination.x)))*180/PI);
                        }

                        break;

                    }

                }
                if(Angle > -90 && Angle < 90){
                    circle(frame,destination,10,Scalar(0,0,255),2,8,0);
                    line(frame,destination,mc[i],Scalar(0,255,0),2,8,0);
                    Prev_destination = destination;
                    Prev_Angle = Angle;
                }
            }

            else {
                data_Ready =0;
            }

            prev_mc = mc[i];
        }
        //cout << "angle "  <<Angle << endl;

        if(data_Ready == 1){
            if(Angle < 90 && Angle > -90){
            sender5.sendSetPointData(Point(destination.x - frame.cols/2,
                                           destination.y - frame.rows/2),Angle,QRdataReady);
            }
        }
        else {
            if(Angle < 90 && Angle > -90){
            sender5.sendSetPointData(Point(0,
                                           0),0,QRdataReady);
            }
        }

    //    cout << "delay of detec " << t2.elapsed() << endl;
    //    cout << "  1 :  " << counter_if_1 << endl;
    //    cout << "  2 :  " << counter_if_2 << endl;
    //    cout << "  3 :  " << counter_if_3 << endl;
    //    cout << "  4 :  " << counter_if_4 << endl;
    //    cout << "  5 :  " << counter_if_5 << endl;
    }

    //////////////detect circle window
    else if (QRdataReady == 1) {
        findContours(frame,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE);
        vector < vector< Point >> contours_poly(contours.size());
        //Gray To BGR
        cvtColor(frame,frame,CV_GRAY2BGR);

        //drawContours(frame,contours,-1,Scalar(0,255,0),3);

        //cout << "    " <<contours.size() << endl;

        for (unsigned int i =0 ; i<contours.size() ; i++ ) {

            approxPolyDP(contours[i],contours_poly[i],3,true);

        }

        /// Get the moments
        vector <Moments>  mu(contours_poly.size() );
        for(unsigned int i = 0; i < contours_poly.size(); i++ ){
            mu[i] = moments( contours_poly[i], false );
        }

        ///  Get the mass centers:
        vector<Point2f> mc( contours_poly.size() );
        for(unsigned int i = 0; i < contours_poly.size(); i++ ){
            contours_area = contourArea(contours_poly[i]);
            if (contours_area > MinArea_Circle) {
                mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
                circle(frame,mc[i],10,Scalar(0,50,200),1,8,0);
                sender5.sendSetPointData(Point(mc[i].x - frame.cols/2,
                                               mc[i].y - frame.rows/2),0,QRdataReady);
            }
        }

    }
    imshow("line",frame);

    //cout << "line : " << t2.elapsed() << endl;
}

void LineDetec::get_QR_flag(char flag)
{
    QRdataReady = flag;
}
///////////////
float LineDetec::DistanceTwoPoint(Point A, Point B)
{
    X_distance = A.x - B.x;
    Y_distance = A.y - B.y;
    distance = sqrt(X_distance*X_distance + Y_distance*Y_distance);
    return  distance;
}
//////////////////
void LineDetec::BubbleSort(float array[],int n){
    bool swapped = true;
       int j = 0;
       float tmp;
       while (swapped) {
           swapped = false;
           j++;
           for (int i = 0; i < n - j; i++) {
               if (array[i] > array[i + 1]) {
                   tmp = array[i];
                   array[i] = array[i + 1];
                   array[i + 1] = tmp;
                   swapped = true;
               }
           }
       }
}
/////////////////
void LineDetec::bubbleSort_point_x(Point arr[], int n)
{
    bool swapped = true;
    int j = 0;
    Point tmp;
    while (swapped) {
        swapped = false;
        j++;
        for (int i = 0; i < n - j; i++) {
            if (arr[i].x > arr[i + 1].x) {
                tmp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}
/////////////////
void LineDetec::bubbleSort_point_y(Point arr[], int n)
{
    bool swapped = true;
    int j = 0;
    Point tmp;
    while (swapped) {
        swapped = false;
        j++;
        for (int i = 0; i < n - j; i++) {
            if (arr[i].y > arr[i + 1].y ) {
                tmp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}
///////////////////
void LineDetec::emit_data(int Flag,Mat image)
{
    if(Flag == 1){
        emit findLine(Point(destination.x - image.cols/2,
                      destination.y - image.rows/2),Angle);
    }
    else if (Flag == 0) {
        emit findLine(Point(Prev_destination.x - image.cols/2,
                      Prev_destination.y - image.rows/2),Prev_Angle);
    }
}
