cmake_minimum_required(VERSION 3.16)
project(swayler
    VERSION 0.1.0
    DESCRIPTION "Modern theme manager for Sway"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
pkg_check_modules(LIBADWAITA REQUIRED libadwaita-1)
find_package(nlohmann_json REQUIRED)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
set(SOURCES
    src/main.cpp
    src/application.cpp
    src/window.cpp
    src/theme_manager.cpp
)

set(HEADERS
    src/application.hpp
    src/window.hpp
    src/theme_manager.hpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GTKMM_INCLUDE_DIRS}
    ${LIBADWAITA_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GTKMM_LIBRARIES}
    ${LIBADWAITA_LIBRARIES}
    nlohmann_json::nlohmann_json
)

# Compiler flags from pkg-config
target_compile_options(${PROJECT_NAME} PRIVATE
    ${GTKMM_CFLAGS_OTHER}
    ${LIBADWAITA_CFLAGS_OTHER}
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${GTKMM_LIBRARY_DIRS}
    ${LIBADWAITA_LIBRARY_DIRS}
)

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES data/com.github.swayler.desktop DESTINATION share/applications)