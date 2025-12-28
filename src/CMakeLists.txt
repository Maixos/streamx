# ------------------------------------------------------------
# module name
# ------------------------------------------------------------
set(MODULE_NAME common)

# ------------------------------------------------------------
# sources
# ------------------------------------------------------------
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS *.cpp)

# ------------------------------------------------------------
# object library
# ------------------------------------------------------------
add_library(${MODULE_NAME} OBJECT ${SRCS})

target_include_directories(${MODULE_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${MODULE_NAME} PUBLIC maixos::toolkitx maixos::rtspx)
