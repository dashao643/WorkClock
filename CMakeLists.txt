cmake_minimum_required(VERSION 3.19)

project(WorkClock
	VERSION 1.3.2
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS 
	Core Gui Widgets 
	Sql Charts Network
)

qt_standard_project_setup()

if(CMAKE_BUILD_TYPE STREQUAL Release)
	set(EXE_FLAGS WIN32)
else()
	set(EXE_FLAGS "")
endif()

qt_add_executable(WorkClock
	${EXE_FLAGS}

	src/main.cpp
	src/appconfig.cpp
	src/widget.cpp
	src/recordchart.cpp
	src/recordmodel.cpp
    src/windows_manager.cpp
	src/dialog/targetdialog.cpp
	src/dialog/timedialog.cpp
	src/dialog/tmessagebox.cpp
    
	assets/assets.qrc
	assets/app_ico.rc
)

target_include_directories(WorkClock PRIVATE 
	src/dialog
	src
)

target_compile_definitions(WorkClock PRIVATE
	APP_VERSION="${PROJECT_VERSION}"
)

target_link_libraries(WorkClock PRIVATE
	Qt::Core
	Qt::Gui
	Qt::Widgets
	Qt::Sql
	Qt::Charts
	Qt::Network
)
