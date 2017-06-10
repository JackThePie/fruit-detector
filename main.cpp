/* Program for detecting apples on the tree using OpenCV and different methods: HSV treshold, Hough circle detection, blob detection and Canny edge detection.
main file in mainwindow.cpp
by Jacek Bera */


#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

