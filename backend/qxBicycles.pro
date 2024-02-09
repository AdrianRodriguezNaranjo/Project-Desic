include(../../QxOrm.pri)

TEMPLATE = app
DEFINES += _BUILDING_QX_BICYCLES
INCLUDEPATH += ../../../QxOrm/include/
DESTDIR = ../../../QxOrm/test/_bin/
LIBS += -L"../../../QxOrm/lib"

!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
PRECOMPILED_HEADER = ./include/precompiled.h
} # !contains(DEFINES, _QX_NO_PRECOMPILED_HEADER)

greaterThan(QT_MAJOR_VERSION, 4) {
QT += widgets
QT += quick
QT += qml
QT += httpserver
} else {
QT += declarative
}

macx:CONFIG-=app_bundle

CONFIG(debug, debug|release) {
LIBS += -l"QxOrmd"
} else {
LIBS += -l"QxOrm"
} # CONFIG(debug, debug|release)

HEADERS += ./include/precompiled.h \
    src/bicycleroutes.h
HEADERS += ./include/export.h
HEADERS += ./include/bicycle.h

SOURCES += ./src/bicycle.cpp \
    src/bicycleroutes.cpp
SOURCES += ./src/main.cpp

