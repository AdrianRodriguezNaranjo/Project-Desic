include(../../../../QxOrm.pri)

TEMPLATE = app
DEFINES += _BUILDING_QX_BUS
INCLUDEPATH += ../../../../../QxOrm/include/
DESTDIR = ../../../../../QxOrm/test/_bin/
LIBS += -L"../../../../../QxOrm/lib"

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
    database.cpp \
    http_server.cpp \
    line.cpp \
    main.cpp \
    schedule.cpp \
    user.cpp

HEADERS += \
    database.h \
    export.h \
    http_server.h \
    line.h \
    precompiled.h \
    schedule.h \
    user.h
