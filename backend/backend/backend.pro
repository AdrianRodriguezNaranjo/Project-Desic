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
    line.cpp \
    main.cpp \
    routes_bus_stop.cpp \
    routes_line.cpp \
    routes_schedule.cpp \
    routes_user.cpp \
    schedule.cpp \
    user_lines.cpp

HEADERS += \
    bus_stop.h \
    bus_user.h \
    chatserver.h \
    database.h \
    export.h \
    line.h \
    precompiled.h \
    routes_bus_stop.h \
    routes_line.h \
    routes_schedule.h \
    routes_user.h \
    schedule.h \
    user_lines.h

