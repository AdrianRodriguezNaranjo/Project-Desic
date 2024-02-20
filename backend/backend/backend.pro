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
    bus_user.cpp \
    chatserver.cpp \
    database.cpp \
    http_server.cpp \
    line.cpp \
    main.cpp \
    schedule.cpp \
    user_lines.cpp

HEADERS += \
    bus_stop.h \
    bus_user.h \
    chatserver.h \
    database.h \
    export.h \
    http_server.h \
    line.h \
    precompiled.h \
    schedule.h \
    user_lines.h

