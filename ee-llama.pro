QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/documents.cpp \
    src/schlibdoc.cpp

HEADERS += \
    include/mainwindow.h \
    include/documents.h \
    include/menuadder.h \
    include/schlibdoc.h

INCLUDEPATH += \
    include \
    ../emdi/emdilib/include \
    'C:/Program Files (x86)/Visual Leak Detector/include' \

FORMS += \
    ui/mainwindow.ui


TARGET = ee-llama

debug:   DESTDIR = debug
release: DESTDIR = release


DESTDIR     = $$DESTDIR
OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR     = $$DESTDIR/moc
RCC_DIR     = $$DESTDIR/rcc
UI_DIR      = $$DESTDIR/ui


CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../emdi/emdilib/build-qtemdilib-Desktop_Qt_5_12_6_MSVC2017_64bit-Debug/debug/ -lemdilib
    LIBS += -L$$PWD/'../../Program Files (x86)/Visual Leak Detector/lib/Win64/' -lvld
    PRE_TARGETDEPS += $$PWD/../emdi/emdilib/build-qtemdilib-Desktop_Qt_5_12_6_MSVC2017_64bit-Debug/debug/emdilib.lib
    PRE_TARGETDEPS += $$PWD/'../../Program Files (x86)/Visual Leak Detector/lib/Win64/vld.lib'
    ee-llama.depends = emdilib
}

CONFIG(release, debug|release) {
    LIBS += -L$$PWD/../emdi/emdilib/build-qtemdilib-Desktop_Qt_5_12_6_MSVC2017_64bit-Release/release/ -lemdilib
    #LIBS += -L$$PWD/'../../Program Files (x86)/Visual Leak Detector/lib/Win64/' -lvl
    PRE_TARGETDEPS += $$PWD/../emdi/emdilib/build-qtemdilib-Desktop_Qt_5_12_6_MSVC2017_64bit-Release/release/emdilib.lib
    #PRE_TARGETDEPS += $$PWD/'../../Program Files (x86)/Visual Leak Detector/lib/Win64/vl.lib'
    ee-llama.depends = emdilib
}


DEPENDPATH += $$PWD/'../../Program Files (x86)/Visual Leak Detector/include'

RESOURCES += \
    llamasource.qrc

