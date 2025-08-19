QT       += core gui
QT += uitools
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    logger.cpp \
    login.cpp \
    main.cpp \
    md5.cpp \
    searchfile.cpp \
    showsearchfile.cpp \
    threadpool.cpp \
    widget.cpp

HEADERS += \
    Packdef.h \
    json.hpp \
    logger.h \
    login.h \
    md5.h \
    searchfile.h \
    showsearchfile.h \
    threadpool.h \
    widget.h

FORMS += \
    login.ui \
    pickLink.ui \
    searchfile.ui \
    showsearchfile.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(./client/client.pri)
include(./kernel/kernel.pri)

INCLUDEPATH += $$PWD/spdlog/include

RESOURCES += \
    resource.qrc
