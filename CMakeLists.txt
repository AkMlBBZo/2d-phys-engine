cmake_minimum_required(VERSION 3.16)
project(2dEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# Для SFML
find_package(SFML 3.0 COMPONENTS 
    Window 
    Graphics 
    Audio 
    Network 
    REQUIRED
)

# not in use now
# # Logger
# set(myLibsDir "D:/cppProjects/!myLibs")
# if(NOT EXISTS "${myLibsDir}/logger/CMakeLists.txt")
#     message(FATAL_ERROR "Logger not found!")
# endif()
# add_subdirectory("${myLibsDir}/logger" "${CMAKE_BINARY_DIR}/logger")

# log paths
set(LOGS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.logs")
set(MAIN_LOG_FILE "${LOGS_DIR}/logs.txt")
# app name
set(APP_NAME "engine")

# adding log files
file(MAKE_DIRECTORY "${LOGS_DIR}")
file(APPEND "${MAIN_LOG_FILE}" "")

# adding data to config
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    @ONLY
)

# config.h will be here
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Добавляем пути к заголовочным файлам
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # Добавляем путь к вашей папке include
)

# Если у вас есть исходные файлы в src, добавляем их
file(GLOB_RECURSE ENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/engine/*.cpp"
)

# Основной исполняемый файл
add_executable(engine
    main.cpp
    ${ENGINE_SOURCES}
)

target_link_libraries(engine PRIVATE
    # logger # not in use now
    SFML::Window
    SFML::Graphics
    SFML::Audio
    SFML::Network
)