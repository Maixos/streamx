# ============================================================
# Maixos platform / SDK environment
# ============================================================
set(MAIXOS_ROOT "/opt/maixos" CACHE PATH "Maixos SDK root")

# ---- ABI naming (policy, not auto string) ----
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
    set(MAIXOS_ARCH linux-x86_64)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(MAIXOS_ARCH linux-aarch64)
else ()
    message(FATAL_ERROR "Unsupported Maixos architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

if (NOT DEFINED CMAKE_PREFIX_PATH)
    list(APPEND CMAKE_PREFIX_PATH ${MAIXOS_ROOT}/lib/${MAIXOS_ARCH}/cmake)
endif ()

set(ENV{PKG_CONFIG_SYSROOT_DIR} /opt/maixos-dev/staging)
set(ENV{PKG_CONFIG_PATH}
        "${MAIXOS_ROOT}/lib/${MAIXOS_ARCH}/pkgconfig:$ENV{PKG_CONFIG_PATH}"
)

# ------------------------------------------------------------
# dependencies
# ------------------------------------------------------------
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(rtspx REQUIRED)
find_package(rkffmpeg REQUIRED)
find_package(toolkitx REQUIRED)

pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
        libavcodec libavfilter libavformat libavutil libswscale libswresample libavdevice
)
