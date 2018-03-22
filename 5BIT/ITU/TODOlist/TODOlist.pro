#-------------------------------------------------
#
# Project created by QtCreator 2016-12-12T12:52:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += core gui sql network widgets multimedia

TARGET = TODOlist
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addcategory.cpp \
    alarm.cpp \
    edittask.cpp \
    insertitem.cpp \
    licensedialog.cpp \
    loginscreen.cpp

HEADERS  += mainwindow.h \
    addcategory.h \
    alarm.h \
    edittask.h \
    insertitem.h \
    licensedialog.h \
    loginscreen.h

FORMS    += mainwindow.ui \
    addcategory.ui \
    alarm.ui \
    edittask.ui \
    insertitem.ui \
    licensedialog.ui \
    loginscreen.ui

RESOURCES += \
    To-Do-List-Manager.qrc \
    qdarkstyle/style.qrc
