cmake_minimum_required(VERSION 3.8)
include($ENV{cpp_lib_build_scripts_path}/cmake-module/setup.cmake)
include(target_import_boost)
include(target_import_base)

set(ProjectName thread)
project(${ProjectName})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

# 编译目标
add_library(${ProjectName})
target_import_src(${ProjectName})
target_import_boost(${ProjectName} PUBLIC)
target_import_base(${ProjectName} PUBLIC)

set(CMAKE_INSTALL_PREFIX ${cpp_lib_build_scripts_path}/${platform}/.libs/${ProjectName}/)