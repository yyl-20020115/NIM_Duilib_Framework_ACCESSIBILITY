cmake_minimum_required(VERSION 3.10)

project(examples)

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(base)
add_subdirectory(duilib)
add_subdirectory(ui_components)
add_subdirectory(examples/basic)
add_subdirectory(examples/controls)
add_subdirectory(examples/datetime)
add_subdirectory(examples/layouts)
add_subdirectory(examples/move_control)
add_subdirectory(examples/richlist)
add_subdirectory(examples/virtualbox)
add_subdirectory(duilib_uia_proxy)
add_subdirectory(duilib_uia_caller)

