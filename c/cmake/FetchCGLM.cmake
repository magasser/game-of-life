include(FetchContent)

message("-- Fetching CGLM")

FetchContent_Declare(
    cglm
    GIT_REPOSITORY  https://github.com/recp/cglm.git
    GIT_TAG         v0.8.5
)

set(CGLM_SHARED OFF CACHE BOOL "CGLM: Build with a shared library" FORCE)
set(CGLM_STATC ON CACHE BOOL "CGLM: Build with a static library" FORCE)
set(CGLM_USE_C99 ON CACHE BOOL "CGLM: Use C99" FORCE)
set(CGLM_USE_TESTS OFF CACHE BOOL "CGLM: Enable tests" FORCE)

FetchContent_MakeAvailable(cglm)

message("-- Fetching CGLM - done")