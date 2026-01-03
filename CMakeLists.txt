cmake_minimum_required(VERSION 3.16)

project(KioskServer1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Quick Qml Sql)

qt_add_executable(appKioskServer1
    main.cpp
    echo_server.cpp
    echo_server.h
    TableManager.cpp
    TableManager.h
    DbManager.cpp
    DbManager.h
)

qt_add_qml_module(appKioskServer1
    URI KioskServer1
    VERSION 1.0
    QML_FILES main.qml
    SOURCES TableManager.h TableManager.cpp
    SOURCES DbManager.h DbManager.cpp
    QML_FILES ServerPage.qml
    QML_FILES TablePage.qml
    QML_FILES HeaderBar.qml
    QML_FILES TableCard.qml
    QML_FILES TableDetailPage.qml
    QML_FILES
    RESOURCES
    SOURCES
)

target_link_libraries(appKioskServer1
    PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::Qml
        Qt6::Network
        Qt6::Sql
)

# (있으면 좋음)
qt_finalize_executable(appKioskServer1)
