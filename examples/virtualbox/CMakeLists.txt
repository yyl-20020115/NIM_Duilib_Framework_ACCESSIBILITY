cmake_minimum_required(VERSION 3.10)

PROJECT(virtualbox)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../ui_components)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} DIR_LIB_SRC)

add_executable(${PROJECT_NAME} ${DIR_LIB_SRC})
target_link_libraries(${PROJECT_NAME} base duilib ui_components)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

if (MSVC)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_HOME_DIRECTORY}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_HOME_DIRECTORY}/bin"
    )
endif (MSVC)
