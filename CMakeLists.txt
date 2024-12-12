cmake_minimum_required(VERSION 3.16)

project(HeartstringsResonance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES




)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HeartstringsResonance
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        client/windows/basewindow.cpp client/windows/basewindow.h client/windows/basewindow.ui
        client/windows/mainwindow.cpp client/windows/mainwindow.h client/windows/mainwindow.ui
        client/main.cpp
        resource/res.qrc
        resource/shaders.qrc
        client/windows/configwindow.cpp client/windows/configwindow.h client/windows/configwindow.ui
        client/windows/aboutwindow.cpp client/windows/aboutwindow.h client/windows/aboutwindow.ui
        client/windows/rankwindow.cpp client/windows/rankwindow.h client/windows/rankwindow.ui
        client/windows/gamewindow.cpp client/windows/gamewindow.h client/windows/gamewindow.ui
        client/windows/gamewindow.ui
        client/windows/pausewindow.h client/windows/pausewindow.cpp client/windows/pausewindow.ui

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET HeartstringsResonance APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(HeartstringsResonance SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(HeartstringsResonance
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(HeartstringsResonance PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.HeartstringsResonance)
endif()
set_target_properties(HeartstringsResonance PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS HeartstringsResonance
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HeartstringsResonance)
endif()
