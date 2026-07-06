cmake_minimum_required(VERSION 3.19)

project(WorkClock
    VERSION 1.3.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Sql Charts)

qt_standard_project_setup()

qt_add_executable(WorkClock
    MACOSX_BUNDLE           # 启用qDebug()
    # WIN32 MACOSX_BUNDLE     # 关闭qDebug()

    src/main.cpp

    src/dataStructure.h

    src/appconfig.cpp
    src/appconfig.h

    src/widget.cpp
    src/widget.h
    src/widget.ui

    src/recordchart.cpp
    src/recordchart.h

    src/recordmodel.cpp
    src/recordmodel.h

    src/dialog/targetdialog.cpp
    src/dialog/targetdialog.h
    src/dialog/targetdialog.ui

    src/dialog/timedialog.cpp
    src/dialog/timedialog.h
    src/dialog/timedialog.ui

    src/dialog/tmessagebox.cpp
    src/dialog/tmessagebox.h

    assets/assets.qrc
    assets/app_ico.rc
)

target_include_directories(WorkClock PRIVATE src)

target_compile_definitions(WorkClock PRIVATE
    APP_VERSION="${PROJECT_VERSION}"
)

target_link_libraries(WorkClock
    PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Sql
        Qt::Charts
)

include(GNUInstallDirs)

install(TARGETS WorkClock
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET WorkClock
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
