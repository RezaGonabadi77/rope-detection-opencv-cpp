#include "senddataudp.h"

sendDataUDP::sendDataUDP(QObject *parent) : QObject(parent)
{
    recieve_ip = new QHostAddress("192.168.4.1");
}
void sendDataUDP::sendSetPointData(cv::Point point,int angle,char flag){

    int deltaX = point.x;
    int deltaY = point.y;
    int Angle  =  angle;
    char Flag = flag;
    union Data{
        int number;
        char array[2];
    };
    union Data data1;
    union Data data2;
    union Data data3;

    char sendingBuff[44];
    int index = 2;
    char zeroChar;
    bool zeroCharFound = true;

    sendingBuff[0] = 's';
    sendingBuff[1] = 'S';

    data1.number =  deltaX;
    data2.number = deltaY;
    data3.number = Angle;


    sendingBuff[index] = data1.array[0];
    sendingBuff[index+1] = data1.array[1];
    sendingBuff[index+2] = data2.array[0];
    sendingBuff[index+3] = data2.array[1];
    sendingBuff[index+4] = data3.array[0];
    sendingBuff[index+5] = data3.array[1];
    sendingBuff[index+6] = Flag;



    for(int j=9 ; j<38 ; j++){
            sendingBuff[j] = 0;
        }

    for (int i=65 ; i<115 ; i++){
        zeroChar = char(i);
        for(int j=2 ; j<39 ; j++){
            if(zeroChar == sendingBuff[j]){
                zeroCharFound = false;
            }
        }
        if(zeroCharFound == true){
            break;
        }
        zeroCharFound = true;
    }


    for(int i=2 ; i<38 ; i++){
            if(sendingBuff[i]== 0){
                sendingBuff[i] = zeroChar;
            }
        }

    sendingBuff[38] = zeroChar;
    sendingBuff[39] = 'e';

    QUdpSocket udpSocket;

    udpSocket.writeDatagram(sendingBuff, 40,*recieve_ip,51919);
    udpSocket.flush();
   // udpSocket.writeDatagram(sendingBuff , 40 , "192.168.4.1" , 51919);
}
