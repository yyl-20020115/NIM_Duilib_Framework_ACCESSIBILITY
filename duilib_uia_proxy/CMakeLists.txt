cmake_minimum_required(VERSION 3.10)
project(duilib_uia_proxy)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../)

add_definitions(-DENABLE_UIAUTOMATION)
set(SRC_FILES
dllmain.cpp
StdAfx.h
UIABoxProvider.cpp
UIABoxProvider.h
UIAButtonProvider.cpp
UIAButtonProvider.h
UIACheckBoxProvider.cpp
UIACheckBoxProvider.h
UIAComboBoxProvider.cpp
UIAComboBoxProvider.h
UIAControlProvider.cpp
UIAControlProvider.h
UIALabelProvider.cpp
UIALabelProvider.h
UIAListBoxItemProvider.cpp
UIAListBoxItemProvider.h
UIAListBoxProvider.cpp
UIAListBoxProvider.h
UIAProgressProvider.cpp
UIAProgressProvider.h
UIARichEditProvider.cpp
UIARichEditProvider.h
UIAScrollableBoxProvider.cpp
UIAScrollableBoxProvider.h
UIAScrollBarProvider.cpp
UIAScrollBarProvider.h
UIATreeNodeProvider.cpp
UIATreeNodeProvider.h
UIAWindowProvider.cpp
UIAWindowProvider.h
)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} comctl32 gdiplus shlwapi imm32 Msimg32 Oleacc)
set_target_properties(${PROJECT_NAME} PROPERTIES
    FOLDER "${PROJECT_NAME}"
    OUTPUT_NAME ${PROJECT_NAME}
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
