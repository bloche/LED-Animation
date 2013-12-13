#Project file created by Grant Boomer. Hopefully it works

QT       += core
QT       += network

QT       -= gui

TARGET = LEDA
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += debug

TEMPLATE = app


SOURCES += main.cpp \
    ./led_interface/ledmatrixdriver.cpp \
    ./led_interface/leddriver.cpp \
   # ./led_interface/videocontroller.cpp \
    ./video_stream_interface/videodisplay.cpp \
    ./video_stream_interface/videoreader.cpp \
    ./video_stream_interface/ledvideoreader.cpp \
    ./youtube_interface/yt-surl.c
HEADERS += \
    ./led_interface/leddriver.h \
    ./led_interface/ledmatrixdriver.h \
   # ./led_interface/videocontroller.h \
    ./video_stream_interface/videodisplay.h \
    ./video_stream_interface/videoreader.h \
    ./video_stream_interface/videoreaderdelegate.h \
    ./video_stream_interface/ledvideoreader.h \
    ./youtube_interface/yt-surl.h

QMAKE_CXXFLAGS += -std=c++0x `pkg-config opencv --cflags --libs` -lcurl

#DEFINES += USE_STD_OUT

unix:!macx|win32: LIBS += -L$$PWD/../build-RPLDCommunications/ -lRPLDCommunications
LIBS += -lwiringPi

INCLUDEPATH += $$PWD/../rpldcommunications
DEPENDPATH += $$PWD/../rpldcommunications
