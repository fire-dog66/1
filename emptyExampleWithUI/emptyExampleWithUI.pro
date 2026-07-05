QT += widgets

TEMPLATE = lib

DEFINES += EMPTYEXAMPLEWITHUI_LIBRARY

TRANSLATIONS = \

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        changestylethread.cpp \
        emptyExampleWithUI.cpp \
        interface.cpp \
        logic.cpp \
        vSDK.cpp

HEADERS += \
        changestylethread.h \
        emptyExampleWithUI.h \
        emptyExampleWithUI_global.h \
        interface.h \
        logic.h \
        vSDK.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    interface.ui

RESOURCES += \
    Resource.qrc

# 修改至vSDK.dll相同目录
DESTDIR = $$quote(D:\DFX MetaLab\DFX MetaLab)
# 修改dll动态库名
TARGET = emptyExampleWithUI
