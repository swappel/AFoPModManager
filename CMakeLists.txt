cmake_minimum_required(VERSION 3.10)
project(AFoPModManager)

set(CMAKE_CXX_STANDARD 17)

# Output executable to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/modManagerEmbedded)

# Add subdirectory (e.g., static library or module)
add_subdirectory(packer)

# Find all .cpp source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the main executable
add_executable(AFoPModManager ${SOURCES})

# Include headers
target_include_directories(AFoPModManager PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link with the packer target
target_link_libraries(AFoPModManager PRIVATE packer)

# Platform-specific compiler and linker flags
if (WIN32)
    # Fully static link on Windows to avoid runtime DLLs
    target_compile_options(AFoPModManager PRIVATE -static-libgcc -static-libstdc++ -pthread)
    target_link_options(AFoPModManager PRIVATE -static -static-libgcc -static-libstdc++ -pthread)
endif()
