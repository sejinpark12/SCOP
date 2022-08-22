#
# Created by Daemyung Jang on 2021/08/29.
#

cmake_minimum_required(VERSION 3.18)
project(OpenGLES LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/Modules)

add_subdirectory(Base)
#add_subdirectory(Test)
add_subdirectory(SCOP)
