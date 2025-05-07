project(Packer LANGUAGES C)

# Define the library
add_library(packer STATIC
    src/FileUtils.cpp
    src/ModArchive.cpp
    src/Packer.cpp
)

# Public headers available to anyone using packer
target_include_directories(packer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)