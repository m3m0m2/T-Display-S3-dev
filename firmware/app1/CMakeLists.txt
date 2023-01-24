cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(app1)

idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
