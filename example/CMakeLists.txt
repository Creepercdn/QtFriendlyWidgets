cmake_minimum_required(VERSION 3.2)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(FriendlyWnd FriendlyWidgets.cpp)

target_link_libraries(FriendlyWnd PUBLIC Qt6::Widgets QtFriendlyWidgets)

add_custom_command(TARGET FriendlyWnd POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:FriendlyWnd> $<TARGET_FILE_DIR:FriendlyWnd>
    COMMAND_EXPAND_LISTS
)

include(windeployqt.cmake)

windeployqt(FriendlyWnd ${CMAKE_INSTALL_BINDIR})
