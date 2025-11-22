cmake_minimum_required(VERSION 3.20)

project(FlyFFResourceFramework
    VERSION 0.1.0
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# C++ Einstellungen
# ---------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-Warnings
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---------------------------------------------------------------------------
# Qt 6.9.3 finden
# ---------------------------------------------------------------------------

# Widgets = für dein UI
# Gui     = Icons, Pixmaps, Fonts, Painter
# Core    = QByteArray, QFile, QString etc.

find_package(Qt6 6.9.3 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

# Automoc/UIC/RCC aktivieren (Qt benötigt das)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---------------------------------------------------------------------------
# Quellcode finden
# ---------------------------------------------------------------------------

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.qrc"
)

# ---------------------------------------------------------------------------
# Executable erzeugen
# ---------------------------------------------------------------------------

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

# Includes setzen
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Mit Qt linken
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# Unter Windows: WinMain erzeugen
if (WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE /ENTRY:mainCRTStartup)
endif()
