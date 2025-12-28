# ============================================================
# Maixos Environment Setup
# ============================================================
# This CMake script provides a professional and reusable way
# to configure external dependencies for the Maixos project.
# It supports two modes:
#   1. Release Mode: using system-installed packages (apt or prebuilt)
#   2. Development Mode: using local development libraries
# ============================================================

cmake_minimum_required(VERSION 3.16)
include_guard(GLOBAL)

# ------------------------------------------------------------
# Root and Architecture Settings
# ------------------------------------------------------------
# Define the root directory for Maixos SDK
set(MAIXOS_ROOT "/opt/maixos" CACHE PATH "Maixos SDK root")

# Automatically set the target architecture based on the system processor
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
    set(MAIXOS_ARCH linux-x86_64)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(MAIXOS_ARCH linux-aarch64)
else ()
    message(FATAL_ERROR "Unsupported Maixos architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

# ------------------------------------------------------------
# System dependencies
# ------------------------------------------------------------
find_package(OpenCV REQUIRED)
find_package(yaml-cpp REQUIRED)

# ------------------------------------------------------------
# Release Mode: find prebuilt runtime packages
# ------------------------------------------------------------
message(STATUS "Building in release mode. Using system-installed packages.")

# Prepend Maixos SDK CMake paths so find_package can discover SDK packages
list(APPEND CMAKE_PREFIX_PATH "${MAIXOS_ROOT}/lib/${MAIXOS_ARCH}/cmake")

# Prepend Maixos SDK pkg-config paths so pkg-config can discover SDK libraries
set(ENV{PKG_CONFIG_PATH} "${MAIXOS_ROOT}/lib/${MAIXOS_ARCH}/pkgconfig:$ENV{PKG_CONFIG_PATH}")

# Find packages via CMake
find_package(rtspx REQUIRED)
find_package(rkffmpeg REQUIRED)
find_package(toolkitx REQUIRED)

# Find packages via pkg-config
