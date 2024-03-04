#include "colordetection.h"

colorDetection::colorDetection(QObject *parent) : QObject(parent)
{
    do_Hist_And_Backproj();
    namedWindow("final color",WINDOW_NORMAL);
    //BLUE
    /*createTrackbar("hue blue","final color",&low_hue_blue,high_hue_blue);
    createTrackbar("sat blue","final color",&low_sat_blue,high_sat_blue);
    createTrackbar("value blue","final color",&low_value_blue,high_value_blue);
    createTrackbar("erode","final color",&erosion_size,max_kernel_size);
    createTrackbar("dilate","final color",&dilation_size,max_kernel_size);
    createTrackbar("erode type","final color",&erosion_elem,max_elem);
    createTrackbar("dilate elem","final color",&dilation_elem,max_elem);*/

    //GREEN
    createTrackbar("hue pink","final color",&low_hue_pink,high_hue_pink);
    createTrackbar("max hue pink","final color",&high_hue_pink,MAX_HSV);
    createTrackbar("sat pink","final color",&low_sat_pink,high_sat_pink);
    createTrackbar("max sat pink","final color",&high_sat_pink,MAX_HSV);
    createTrackbar("value pink","final color",&low_value_pink,high_value_pink);
    createTrackbar("max value pink","final color",&high_value_pink,MAX_HSV);
    createTrackbar("erode","final color",&erosion_size,max_kernel_size);
    createTrackbar("dilate","final color",&dilation_size,max_kernel_size);
    createTrackbar("erode type","final color",&erosion_elem,max_elem);
    createTrackbar("dilate elem","final color",&dilation_elem,max_elem);

    createTrackbar("hue green","final color",&low_hue_green,high_hue_green);
    createTrackbar("max hue","final color",&high_hue_green,MAX_HSV);
    createTrackbar("sat green","final color",&low_sat_green,high_sat_green);
    createTrackbar("max sat","final color",&high_sat_green,MAX_HSV);
    createTrackbar("value green","final color",&low_value_green,high_value_green);
    createTrackbar("max value","final color",&high_value_green,MAX_HSV);



    //namedWindow("frame",WINDOW_NORMAL);
}

void colorDetection::getColorDetection(Mat BP_image)
{
    //remove batter of IP cam
    Remove_green_battery(BP_image);
    //imshow("frame",BP_image);
    final_Backproj = detectBlueColorInrange(BP_image);
    emit colorDetectReady(final_Backproj);
    imshow("final color",final_Backproj);
}

void colorDetection::get_QR_flag_color(char flag)
{
    if( flag == 0){
        green_detect_flag = 1;
        blue_detect_flag = 0;
        pink_detect_flag = 0;
    }
    else if (flag == 1) {
        pink_detect_flag = 1;
        green_detect_flag = 0;
        blue_detect_flag = 0;
    }
}

Mat colorDetection::detectBlueColorInrange(Mat BP_image){

    //t1.start();
    if(!BP_image.empty()){
        //determine time        
        //
        //Blue color
        //imshow("frame",BP_image);
        //filter on raw image first!
        medianBlur(BP_image,BP_image,medianblur_filter_kernel1);
        // RGB to HSV
        cvtColor(BP_image,hsv,CV_BGR2HSV);
        // inRange function on image
        if(blue_detect_flag == 1){
            inRange(hsv,Scalar(low_hue_blue,low_sat_blue,low_value_blue),Scalar(high_hue_blue,high_sat_blue,high_value_blue),inrange_color);        ///blue
        }
        else if (green_detect_flag == 1) {
            inRange(hsv,Scalar(low_hue_green,low_sat_green,low_value_green),Scalar(high_hue_green,high_sat_green,high_value_green),inrange_color);   ///green
        }
        else if (pink_detect_flag == 1) {
            inRange(hsv,Scalar(low_hue_pink,low_sat_pink,low_value_pink),Scalar(high_hue_pink,high_sat_pink,high_value_pink),inrange_color);  ///PINK
        }

       ///green
        //filter on inrange pic
        medianBlur(inrange_color,inrange_color,medianblur_filter_kernel2);
        //erosion and dilation function on pic
        Erosion_Dilation();
        //filter again
        medianBlur(inrange_color,inrange_color,medianblur_filter_kernel3);
        //
        final_Backproj = backprojectionResult(hsv);
        final_Backproj &= inrange_color;
        //imshow("back",backproj);
    }
    //cout << "delay of color code" << t1.elapsed() << endl;

    return inrange_color;
}
void colorDetection::do_Hist_And_Backproj(void){

    hsize = 32;
    hranges[0] = 0;
    hranges[1] = 180;
    phranges = hranges;
    histimg = Mat::zeros(200, 320, CV_8UC3);
    hist=Mat::zeros(hsize,1,CV_32F);
    //BLUE
    if(blue_detect_flag == 1){
        hist.at<float>(0,16)=255;
        hist.at<float>(0,17)=255;
        hist.at<float>(0,18)=255;
        hist.at<float>(0,19)=255;
    }
    //
    //GREEN
    else if(green_detect_flag == 1){
        hist.at<float>(0,9)=255;
        hist.at<float>(0,10)=255;
        hist.at<float>(0,11)=255;
        hist.at<float>(0,12)=255;
        //hist.at<float>(0,14)=255;
    }
    else if (pink_detect_flag == 1) {
        hist.at<float>(0,26)=255;
        hist.at<float>(0,27)=255;
        hist.at<float>(0,28)=255;
        hist.at<float>(0,29)=255;
    }
    histimg = Scalar::all(0);
    int binW = histimg.cols / hsize;
    Mat buf(1, hsize, CV_8UC3);
    for( int i = 0; i < hsize; i++ )
        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
    cvtColor(buf, buf, CV_HSV2BGR);

    for( int i = 0; i < hsize; i++ )
    {
        int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
        rectangle( histimg, Point(i*binW,histimg.rows),
                   Point((i+1)*binW,histimg.rows - val),
                   Scalar(buf.at<Vec3b>(i)), -1, 8 );
    }

//    namedWindow("histogram",WINDOW_NORMAL);
//    imshow("histogram",histimg);

}
Mat colorDetection::backprojectionResult(Mat src){

    int ch[] = {0, 0};
    hue.create(hsv.size(), hsv.depth());
    mixChannels(&src, 1, &hue, 1, ch, 1);
    calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
    return backproj;

}
void colorDetection::Erosion_Dilation(){

    int erosion_type;
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }


    int dilation_type;
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
    else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
    else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }


    ERODE_elem = getStructuringElement( erosion_type,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ));
    DILATE_elem = getStructuringElement( dilation_type,
                                        Size( 2*dilation_size + 1, 2*dilation_size+1 ));

    erode(inrange_color,inrange_color,ERODE_elem);
    dilate(inrange_color,inrange_color,DILATE_elem);

}

void colorDetection::Remove_green_battery(Mat src)
{
    for(int i = 0; i < 20; i++)
    {
        for(int j = 280; j < 320; j++)
        {
            // do something with BGR values...
            src.at<Vec3b>(i,j) = 0;

        }
    }
}
