#-------------------------------------------------
#
# Project created by QtCreator 2018-12-04T09:58:53
#
#-------------------------------------------------

    QT += core gui sql widgets

    QXLSX_PARENTPATH=../xlsx/         # current QXlsx path is . (. means curret directory)
    QXLSX_HEADERPATH=../xlsx/  # current QXlsx header path is ./header/
    QXLSX_SOURCEPATH=../xlsx/  # current QXlsx source path is ./source/
include(../xlsx/qtxlsx.pri)




#greaterThan(QT_MAJOR_VERSION, 4):QT += sql widgets
TARGET = PR_Master
TEMPLATE = app

CONFIG   -= app_bundle

SOURCES += main.cpp\
        mainwindow.cpp \
    dbmanager.cpp \
    xlsxreader.cpp \
    dbworker.cpp

HEADERS  += mainwindow.h \
    dbmanager.h \
    Items_Definition.h \
    xlsxreader.h \
    dbworker.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc
