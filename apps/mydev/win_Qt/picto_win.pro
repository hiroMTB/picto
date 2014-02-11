#-------------------------------------------------
#
# Project created by QtCreator 2014-01-24T10:03:02
#
#-------------------------------------------------

include(openFrameworks.pri)
include(addons.pri)

QT       += core gui
QT       += opengl widgets

#CONFIG += static

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(release, debug|release) {
    TARGET = picto_win
}else{
   TARGET = picto_win
}

TEMPLATE = app

INCLUDEPATH += src
INCLUDEPATH += src/Qt

HEADERS +=  src/Qt/pictoController.h\
            src/Qt/mainWindow.h \
            src/Qt/subWidget.h \
            src/Qt/mainWidget.h\
            src/attractor.h\
            src/gpuPicto.h\
            src/gpuPictoChar.h\
            src/gpuPictoString.h\
            src/ofTextureAdv.h\
            src/pingPongBuffer.h\
            src/testApp.h\

SOURCES +=  src/main.cpp\
            src/Qt/pictoController.cpp\
            src/Qt/mainWindow.cpp\
            src/Qt/mainWidget.cpp\
            src/Qt/subWidget.cpp\
            src/attractor.cpp\
            src/gpuPicto.cpp\
            src/gpuPictoChar.cpp\
            src/gpuPictoString.cpp\
            src/ofTextureAdv.cpp\
            src/pingPongBuffer.cpp\
            src/testApp.cpp\


#RESOURCES += shaders.qrc

FORMS   +=  src/Qt/pictoController.ui

#CONFIG -= warn_on
#QMAKE_CXXFLAGS_DEBUG   = -MDd
#QMAKE_CXXFLAGS_RELEASE = -MD

# .obj directory
CONFIG(release, debug|release) {
    OBJECTS_DIR = ../obj/release
}else{
    OBJECTS_DIR = ../obj/debug
}
