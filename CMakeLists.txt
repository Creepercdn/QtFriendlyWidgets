cmake_minimum_required(VERSION 3.2)

project(QtFriendlyWidgets VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Gui)

add_library(QtFriendlyWidgets STATIC
    src/qadvancedslider.cpp
    src/qadvancedslidercommon.cpp
    src/qadvanceddial.cpp
    include/QtFriendlyWidgets/qadvancedslider.hpp
    include/QtFriendlyWidgets/private/qadvancedslidercommon_p.hpp
    include/QtFriendlyWidgets/qadvanceddial.hpp
)

target_include_directories(QtFriendlyWidgets PUBLIC include/ PRIVATE include/QtFriendlyWidgets)

target_link_libraries(QtFriendlyWidgets PUBLIC Qt6::Widgets Qt6::Gui)

# qt_add_translations(QtFriendlyWidgets TS_FILES QtFriendlyWidgets_zh_CN.ts QM_FILES_OUTPUT_VARIABLE qm_files LUPDATE_OPTIONS "-no-obsolete")

add_subdirectory(example)
