QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appconfig.cpp \
    main.cpp \
    recordmodel.cpp \
    settingdialog.cpp \
    timedialog.cpp \
    widget.cpp

HEADERS += \
    appconfig.h \
    dataStructure.h \
    recordmodel.h \
    settingdialog.h \
    timedialog.h \
    widget.h

FORMS += \
    settingdialog.ui \
    timedialog.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = PicRes/clock1.ico

VERSION = 1.3.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# DEFINES += QT_NO_DEBUG_OUTPUT
