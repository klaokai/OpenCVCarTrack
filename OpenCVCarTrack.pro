#-------------------------------------------------
#
# Project created by QtCreator 2014-11-30T13:30:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCVCarTrack
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
    FGDetector/FGDetector.cpp \
    BlobDetector/BlobDetector.cpp \
    Controller.cpp \
    videoinfo.cpp \
    FGDetector/ocvfgdetector.cpp

HEADERS  += mainwindow.h \
    CarInfo.h \
    FGDetector/FGDetector.h \
    BlobDetector/BlobDetector.h \
    Controller.h \
    videoinfo.h \
    common.h \
    FGDetector/ocvfgdetector.h

FORMS    += mainwindow.ui


win32{
INCLUDEPATH+=D:\opencv\build\include\opencv\
             D:\opencv\build\include\opencv2\
             D:\opencv\build\include\

LIBS+=D:\OpenCV4Qt\lib\libopencv_calib3d2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_contrib2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_core2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_features2d2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_flann2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_gpu2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_highgui2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_imgproc2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_legacy2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_ml2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_objdetect2412d.dll.a\
        D:\OpenCV4Qt\lib\libopencv_video2412d.dll.a
}
