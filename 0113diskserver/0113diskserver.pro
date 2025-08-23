QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Logger.cpp \
        main.cpp \
        redismanager.cpp
HEADERS += \
        redismanager.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(./server/server.pri)
include(./sql/mysql.pri)
include(./kernel/kernel.pri)


HEADERS += \
    Logger.h \
    Packdef.h \
    json.hpp \
    redismanager.h

INCLUDEPATH += $$PWD/spdlog/include

INCLUDEPATH += $$PWD/redis/include
INCLUDEPATH += C:/Users/Lenovo/Desktop/vcpkg/vcpkg/installed/x64-windows/include

# 对于 Debug 构建
CONFIG(debug, debug|release) {
    # 添加 Debug 版本的库路径
    LIBS += -LC:/Users/Lenovo/Desktop/vcpkg/vcpkg/installed/x64-windows/debug/lib
    # 链接 Debug 版本的 hiredis 库 (hiredisd.lib)
    LIBS += -lhiredisd
}

# 对于 Release 构建
CONFIG(release, debug|release) {
    # 添加 Release 版本的库路径
    LIBS += -L-LC:/Users/Lenovo/Desktop/vcpkg/vcpkg/installed/x64-windows/lib
    # 链接 Release 版本的 hiredis 库 (hiredis.lib)
    LIBS += -lhiredis
}

LIBS += -lws2_32
LIBS += -lwinmm



