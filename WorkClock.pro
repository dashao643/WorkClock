QT += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    settingdialog.cpp \
    timedialog.cpp \
    widget.cpp

HEADERS += \
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
    PicRes.qrc

#使用本地相对路径，指向PicRes文件夹下的clock1.ico
RC_ICONS = PicRes/clock1.ico

#版本号遵循 主版本.次版本.修订版本（如 1.2.3）
#主版本：功能大改版
#次版本：新增核心功能
#修订版本：修复 bug、小优化
VERSION = 1.1.3
#把VERSION定义成预编译宏，传递到代码中
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

