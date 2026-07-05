cmake_minimum_required(VERSION 3.19)

project(WorkClock2
    VERSION 1.3.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动 MOC / UIC / RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Sql Charts)

qt_standard_project_setup()

qt_add_executable(WorkClock2
    WIN32 MACOSX_BUNDLE
    src/appconfig.cpp
    src/main.cpp
    src/recordchart.cpp
    src/recordmodel.cpp
    src/targetdialog.cpp
    src/timedialog.cpp
    src/tmessagebox.cpp
    src/widget.cpp
    src/appconfig.h
    src/dataStructure.h
    src/recordchart.h
    src/recordmodel.h
    src/targetdialog.h
    src/timedialog.h
    src/tmessagebox.h
    src/widget.h
    src/targetdialog.ui
    src/timedialog.ui
    src/widget.ui
    resource.qrc
    app.rc
)

target_include_directories(WorkClock2 PRIVATE src)

target_link_libraries(WorkClock2
    PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Sql
        Qt::Charts
)

target_compile_definitions(WorkClock2 PRIVATE
    # APP_VERSION="${PROJECT_VERSION}"
)

include(GNUInstallDirs)

install(TARGETS WorkClock2
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET WorkClock2
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
