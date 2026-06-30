QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/appconfig.cpp \
    src/main.cpp \
    src/recordmodel.cpp \
    src/targetdialog.cpp \
    src/timedialog.cpp \
    src/tmessagebox.cpp \
    src/widget.cpp \

HEADERS += \
    src/appconfig.h \
    src/dataStructure.h \
    src/recordmodel.h \
    src/targetdialog.h \
    src/timedialog.h \
    src/tmessagebox.h \
    src/widget.h \

FORMS += \
    src/targetdialog.ui \
    src/timedialog.ui \
    src/widget.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

RC_ICONS = PicRes/clock1.ico

VERSION = 1.3.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
