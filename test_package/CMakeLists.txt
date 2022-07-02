cmake_minimum_required(VERSION 3.16)

project(PackageTest CXX)

set(TARGET_NAME "example")

find_package(KF5Archive 5.96.0 REQUIRED)

add_executable(${TARGET_NAME} "example.cpp")
target_link_libraries(${TARGET_NAME} KF5::Archive)
