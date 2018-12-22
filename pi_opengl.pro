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
    cskybox.cpp

HEADERS  += cglwidget.h \
    cmesh.h \
    cmodel.h \
    cskybox.h

FORMS    += cglwidget.ui

LIBS+=-lassimp

RESOURCES += \
    shaders.qrc \
    images.qrc
