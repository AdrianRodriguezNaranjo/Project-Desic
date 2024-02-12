include(../../../../QxOrm.pri)

TEMPLATE = app
DEFINES += _BUILDING_QX_BUS
INCLUDEPATH += ../../../../../QxOrm/include/
DESTDIR = ../../../../../QxOrm/test/_bin/
LIBS += -L"$$PWD/../../../../lib"

!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
PRECOMPILED_HEADER = ./precompiled.h
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

SOURCES += \
    bus_stop.cpp \
    database.cpp \
    http_server.cpp \
    line.cpp \
    main.cpp \
    schedule.cpp

HEADERS += \
    bus_stop.h \
    database.h \
    export.h \
    http_server.h \
    line.h \
    precompiled.h \
    schedule.h

# TARGET = backend.exe


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../release/ -lQxOrmd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../debug/ -lQxOrmd

INCLUDEPATH += $$PWD/../../../../lib
DEPENDPATH += $$PWD/../../../../lib
