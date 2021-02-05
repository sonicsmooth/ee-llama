QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/dbutils.cpp \
    src/extensiondocvisitor.cpp \
    src/filedialogs.cpp \
    src/filesavecopyasvisitor.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/documents.cpp \
    src/menudocvisitor.cpp \
    src/filesavevisitor.cpp \
    src/numberemitter.cpp \
    src/task.cpp

HEADERS += \
    include/dbutils.h \
    include/extensiondocvisitor.h \
    include/filedialogs.h \
    include/filesavecopyasvisitor.h \
    include/idocvisitor.h \
    include/mainwindow.h \
    include/documents.h \
    include/menudocvisitor.h \
    include/filesavevisitor.h \
    include/numberemitter.h \
    include/task.h \
    $$PWD/sqlite3/sqlite3.h \



INCLUDEPATH += \
    include \
    emdi/emdilib/include \
    'C:/Program Files (x86)/Visual Leak Detector/include' \
    sqlite3

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

BUILDNAME = build-qtemdilib-Desktop_Qt_5_15_2_MSVC2019_64bit

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/emdi/emdilib/$$BUILDNAME-Debug/debug/ -lemdilib
    LIBS += -L'c:/Program Files (x86)/Visual Leak Detector/lib/Win64/' -lvld
    LIBS += -L$$PWD/sqlite3/ -lsqlite3
    PRE_TARGETDEPS += $$PWD/emdi/emdilib/$$BUILDNAME-Debug/debug/emdilib.lib
    PRE_TARGETDEPS += 'c:/Program Files (x86)/Visual Leak Detector/lib/Win64/vld.lib'
    PRE_TARGETDEPS += $$PWD/sqlite3/sqlite3.lib
    ee-llama.depends = emdilib
}

CONFIG(release, debug|release) {
    LIBS           += -L$$PWD/emdi/emdilib/$$BUILDNAME-Release/release/ -lemdilib
    LIBS           += -L$$PWD/sqlite3/ -lsqlite3
    PRE_TARGETDEPS += $$PWD/emdi/emdilib/$$BUILDNAME-Release/release/emdilib.lib
    PRE_TARGETDEPS += $$PWD/sqlite3/sqlite3.lib
    ee-llama.depends = emdilib
}


DEPENDPATH += 'c:/Program Files (x86)/Visual Leak Detector/include'
DEPENDPATH += $$PWD/sqlite3

RESOURCES += \
    llamasource.qrc

