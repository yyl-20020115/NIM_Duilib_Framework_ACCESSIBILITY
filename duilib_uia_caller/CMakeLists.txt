cmake_minimum_required(VERSION 3.10)
project(duilib_uia_caller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
# 1: static MFC library, 2: shared MFC library
set(CMAKE_MFC_FLAG 2)

#find_package(MFC)
#if(NOT MFC_FOUND)
#    message(FATAL_ERROR "MFC is not found, please check MFC installation")
#endif()

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../)

add_definitions(-DENABLE_UIAUTOMATION)
set(SRC_FILES
DuilibUIAMFCCaller.rc
Duilib_UIA_MFC_Caller.cpp
Duilib_UIA_MFC_Caller.h
Duilib_UIA_MFC_CallerDlg.cpp
Duilib_UIA_MFC_CallerDlg.h
framework.h
main.cpp
pch.cpp
pch.h
Resource.h
targetver.h
)

add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} comctl32 gdiplus shlwapi imm32 Msimg32 Oleacc)
set_target_properties(${PROJECT_NAME} PROPERTIES
    FOLDER "${PROJECT_NAME}"
    OUTPUT_NAME ${PROJECT_NAME}
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        -DWIN32
        -D_DEBUG
        -D_WINDOWS
        -D_UNICODE
        -DUNICODE
        -D_AFXDLL
        )
target_link_options(${PROJECT_NAME} PRIVATE
        /ENTRY:wWinMainCRTStartup
        )


