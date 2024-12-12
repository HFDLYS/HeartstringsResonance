cmake_minimum_required(VERSION 3.16)

project(HeartstringsResonance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGL OpenGlWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(APPLE)
    include_directories("/opt/homebrew/include")
    link_directories("/opt/homebrew/lib")
endif()

set(PROJECT_SOURCES
    
)

set(GRAPHICS_SOURCES
    client/graphics/DEBUG.h
    client/graphics/animation/animation.cpp client/graphics/animation/animation.h
    client/graphics/animation/animationfactory.cpp client/graphics/animation/animationfactory.h
    client/graphics/animation/animationfall.cpp client/graphics/animation/animationfall.h
    client/graphics/animation/animationremove.cpp client/graphics/animation/animationremove.h
    client/graphics/animation/animationswap.cpp client/graphics/animation/animationswap.h
    client/graphics/animation/animationwait.cpp client/graphics/animation/animationwait.h
    client/graphics/camera.h
    client/graphics/gemmodelfactory.cpp client/graphics/gemmodelfactory.h
    client/graphics/gem.cpp client/graphics/gem.h
    client/graphics/rendermanager.cpp client/graphics/rendermanager.h
    client/graphics/hypercubethread.cpp client/graphics/hypercubethread.h
    client/graphics/mesh.cpp client/graphics/mesh.h
    client/graphics/model.cpp client/graphics/model.h
    client/graphics/gemmanager.cpp client/graphics/gemmanager.h
)

set(CORE_SOURCES
    core/gem.cpp core/gem.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HeartstringsResonance
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${GRAPHICS_SOURCES}
        client/windows/framelesswindow.cpp client/windows/framelesswindow.h client/windows/framelesswindow.ui 
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

target_link_libraries(HeartstringsResonance PRIVATE
    Qt::OpenGL
    Qt::OpenGLWidgets
)

target_link_libraries(HeartstringsResonance PRIVATE assimp)

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

if(APPLE)
    target_include_directories(HeartstringsResonance PRIVATE /opt/homebrew/Cellar/assimp/5.4.3/include)
endif()

if(WIN32)
    target_include_directories(HeartstringsResonance PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/library/assimp/include)
    target_link_libraries(DataStructureFinalProject PRIVATE
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/library/assimp/lib/
        ${CMAKE_CURRENT_SOURCE_DIR}/library/assimp/lib/libassimp.dll.a
        ${CMAKE_CURRENT_SOURCE_DIR}/library/assimp/lib/libassimp.dll
    )
endif()


include(GNUInstallDirs)
install(TARGETS HeartstringsResonance
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HeartstringsResonance)
endif()
