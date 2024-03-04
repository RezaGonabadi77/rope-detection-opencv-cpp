#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <capture.h>
#include <QThread>
#include <linedetec.h>
#include "colordetection.h"
#include "senddataudp.h"
#include "qr_detection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Capture *cam;
    LineDetec *linedetec;
    colorDetection *Colordetect;
    sendDataUDP *send;
    QR_detection *QRobject;

    QThread *cam_thread;
    QThread *linedetec_thread;
    QThread *colordetect_thread;
    QThread *send_thread;

public slots:
    void do_smoothing(int b);

};

#endif // MAINWINDOW_H
