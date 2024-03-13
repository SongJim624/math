cmake_minimum_required(VERSION 3.11.0)

aux_source_directory(. source_unsga)

add_library(static_unsga STATIC ${source_unsga})
add_library(dynamic_unsga SHARED ${source_unsga})