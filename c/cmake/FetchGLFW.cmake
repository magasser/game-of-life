include(FetchContent)

message("-- Fetching GLFW")

FetchContent_Declare(
    glfw
    GIT_REPOSITORY  https://github.com/glfw/glfw.git
    GIT_TAG         3.3.7
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW: Build examples" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW: Build documentation" FORCE)
set(GLFW_BUILD_TEST OFF CACHE BOOL "GLFW: Build tests" FORCE)

FetchContent_MakeAvailable(glfw)

message("-- Fetching GLFW - done")