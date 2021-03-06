#-------------------------------------------------
#
# Project created by QtCreator 2018-12-14T20:14:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pi_opengl
TEMPLATE = app


SOURCES += main.cpp\
        cglwidget.cpp \
    cmesh.cpp \
    cmodel.cpp \
    cskybox.cpp \
    ccamera.cpp \
    mpu6050drv.cpp

HEADERS  += cglwidget.h \
    cmesh.h \
    cmodel.h \
    cskybox.h \
    ccamera.h \
    mpu6050drv.h

FORMS    += cglwidget.ui

LIBS+=-lassimp

RESOURCES += \
    shaders.qrc \
    images.qrc

LIBS+=-lwiringPi
LIBS+=-lcrypt

INCLUDEPATH+="/usr/local/include/bullet"
LIBS+=-lLinearMath -lBulletCollision -lBulletDynamics

