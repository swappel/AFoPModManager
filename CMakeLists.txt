cmake_minimum_required(VERSION 3.10)
project(ManagerUtils)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)


add_subdirectory(packer)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(ManagerUtils ${SOURCES})
target_include_directories(ManagerUtils PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ManagerUtils PRIVATE packer)

