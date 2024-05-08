cmake_minimum_required(VERSION 3.5)

project(word-memorize-software VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia Sql)  # 添加Multimedia模块
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia Sql)  # 添加Multimedia模块

# 解决提升类无法include的难题
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)

set(PROJECT_SOURCES
        main.cpp
        login.ui
        login.cpp
        login.h
        appinit.cpp
        appinit.h
        day.css
        night.css
        student_main.cpp
        student_main.h
        student_main.ui
        word_frame.h
        word_frame.cpp
        word.h
        word_display.cpp
        word_display.h
        word_display.ui
        resource_manager.h
        resource_manager.cpp
        acrylicframe.h
        acrylicframe.cpp
        Image_processing.h
        database.h
        database.cpp
        table.h
        role.h
        role.cpp
        class.h
        class.cpp
        task.h
        task.cpp
        words.h
        words.cpp
        system.h
        system.cpp
        class_frame.h
        class_frame.cpp
        task_frame.h
        task_frame.cpp
        classcue_frame.h
        classcue_frame.cpp
        classcue_frame.ui
        question.h
        question.cpp
        question.ui
        teacher_main.h
        teacher_main.cpp
        teacher_main.ui
        newclass_frame.h
        newclass_frame.cpp
        newclass_frame.ui
        util.h
        cet4.json
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
qt_add_executable(word-memorize-software
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET word-memorize-software APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(word-memorize-software SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(word-memorize-software
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# target_link_libraries(word-memorize-software PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(word-memorize-software PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Multimedia 
    Qt${QT_VERSION_MAJOR}::Sql
)  # 链接Multimedia模块


set_target_properties(word-memorize-software PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS word-memorize-software
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(word-memorize-software)
endif()
