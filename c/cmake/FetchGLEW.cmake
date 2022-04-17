include(FetchContent)

message("-- Fetching GLEW")

FetchContent_Declare(
    glew
    GIT_REPOSITORY  https://github.com/Perlmint/glew-cmake.git
    GIT_TAG         glew-cmake-2.0.0
)

set(glew-cmake_BUILD_STATIC ON CACHE BOOL "GLEW: Build with a static library" FORCE)
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "GLEW: Build with a shared library" FORCE)
set(ONLY_LIBS ON CACHE BOOL "GLEW: Build only libraries" FORCE)

# FetchContent_GetProperties(glew)
# if(NOT glew_POPULATED)
#     FetchContent_Populate(glew)

#     add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(glew)

message("-- Fetching GLEW - done")