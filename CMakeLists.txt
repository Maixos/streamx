cmake_minimum_required(VERSION 3.16)

project(ai_streamx)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_RPATH "$ORIGIN/lib")
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# ################################### EXECUTE ###################################
include(${PROJECT_SOURCE_DIR}/cmake/dependence.cmake)

include_directories(${X3RDPARTY_INCLUDE_DIRS})

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} pthread ${X3RDPARTY_LINK_LIBS})

## ################################### RELEASE ###################################
set(RELEASE_DIR ${PROJECT_SOURCE_DIR}/release)
install(TARGETS ${PROJECT_NAME} DESTINATION ${RELEASE_DIR})
install(FILES ${PROJECT_SOURCE_DIR}/setup.sh DESTINATION ${RELEASE_DIR})
install(FILES ${X3RDPARTY_INSTALL_FILES} DESTINATION ${RELEASE_DIR}/lib)
#install(FILES ${PROJECT_SOURCE_DIR}/config.yaml DESTINATION ${RELEASE_DIR} RENAME .config.yaml)
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/asset DESTINATION ${RELEASE_DIR})
