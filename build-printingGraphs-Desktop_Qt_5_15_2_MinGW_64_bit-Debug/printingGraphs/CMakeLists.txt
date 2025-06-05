cmake_minimum_required(VERSION 3.5)

project(printingGraphs VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем Qt и необходимые компоненты
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts Sql)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    idatareader.h
    idatareader.cpp
    jsonreader.h
    jsonreader.cpp
    sqlreader.h
    sqlreader.cpp
    datacontainer.h
    igraphs.h
    linegraph.h
    linegraph.cpp
    scattergraph.h
    scattergraph.cpp
    readerfactory.h
    readerfactory.cpp
    graphfactory.h
    graphfactory.cpp
    IOCContainer.h
    AppConfigurator.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(printingGraphs
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(printingGraphs SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(printingGraphs
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Подключаем необходимые модули Qt
target_link_libraries(printingGraphs PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Sql
)

set_target_properties(printingGraphs PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS printingGraphs
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(printingGraphs)
endif()
