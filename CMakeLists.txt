cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
AUX_SOURCE_DIRECTORY(src SOURCES)
project(autoslfl VERSION 1.0.0 DESCRIPTION "Automatic Super Life: Franchise Lord minigame player")

add_executable(autoslfl ${SOURCES})