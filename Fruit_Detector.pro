#-------------------------------------------------
#
# Project created by QtCreator 2015-05-08T13:37:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fruit_Detector
TEMPLATE = app

#opencv include files
INCLUDEPATH += C:\opencv-mingw\install\include
INCLUDEPATH += C:\opencv-mingw\install\x64\mingw\bin

#opencv libs

LIBS += -L"C:\opencv-mingw\install\x64\mingw\bin"
LIBS += -lopencv_core2410 -lopencv_highgui2410 -lopencv_imgproc2410 -lopencv_calib3d2410 -lopencv_features2d2410 -lopencv_nonfree2410 -lopencv_objdetect2410 -lopencv_video2410# here just use dll file names without "lib" just add "-l" in front

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
