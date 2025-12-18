cmake_minimum_required(VERSION 3.16)

project(streamx-project VERSION 1.0.0 LANGUAGES CXX)

# ============================================================
# Global build configuration
# ============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_BUILD_RPATH "$ORIGIN/lib")
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")

# Default build type
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type: Debug, Release, RelWithDebInfo" FORCE)
endif ()

include(GNUInstallDirs)

# ============================================================
# Third-party / dependency management
# ============================================================
include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)

# ============================================================
# Output directories
# ============================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${MAIXOS_ARCH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${MAIXOS_ARCH})

# ============================================================
# Install layout policy
# ============================================================
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${MAIXOS_ROOT}" CACHE PATH "Install prefix" FORCE)
endif ()

if (CMAKE_INSTALL_LIBDIR STREQUAL "lib")
    set(CMAKE_INSTALL_LIBDIR "lib/${MAIXOS_ARCH}" CACHE PATH "Library install directory" FORCE)
endif ()

# ============================================================
# Tests
# ============================================================
option(BUILD_EXAMPLES "Build test applications" ON)

if (BUILD_EXAMPLES)
    add_executable(streamx_test src/main.cpp)

    target_link_libraries(streamx_test toolkitx vcodecx::rkffmpeg rtspx)
endif ()
