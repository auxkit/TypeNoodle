cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-includes")
include(GetVersion) # gives us the VERSION variable :D

project(TypeNoodle VERSION ${CURRENT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Quick
    QuickControls2
)

set(QT_QML_GENERATE_QMLLS_INI ON)
qt_policy(SET QTP0001 NEW)

# Add external dependencies
add_subdirectory(external/spdlog)
add_subdirectory(external/json)

# Option to use FreeType (OFF by default, use Qt's font APIs first)
option(USE_FREETYPE "Use FreeType for font parsing" OFF)
if(USE_FREETYPE)
    add_subdirectory(external/freetype)
endif()

# Source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.cxx"
)

file(GLOB_RECURSE HEADERS
    "src/*.h"
    "src/*.hpp"
)

# QML files
file(GLOB_RECURSE QML_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "qml/*.qml"
)

# Resources
set(RESOURCES
    resources/resources.qrc
)

# Create executable with QML module
qt_add_executable(typenoodle
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)


qt_add_qml_module(typenoodle
    URI TypeNoodle
    VERSION ${CURRENT_VERSION}
    QML_FILES ${QML_FILES}
)

# Link libraries
target_link_libraries(typenoodle PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)

if(USE_FREETYPE)
    target_link_libraries(typenoodle PRIVATE freetype)
    target_compile_definitions(typenoodle PRIVATE USE_FREETYPE)
endif()

# Include directories
target_include_directories(typenoodle PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Finalize the executable
qt_finalize_target(typenoodle)

# Copy TypeNoodle QML module metadata to Resources
add_custom_command(TARGET typenoodle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_BUNDLE_DIR:typenoodle>/Contents/Resources/qml/TypeNoodle"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_BINARY_DIR}/TypeNoodle/qmldir"
        "$<TARGET_BUNDLE_DIR:typenoodle>/Contents/Resources/qml/TypeNoodle/qmldir"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_BINARY_DIR}/TypeNoodle/typenoodle.qmltypes"
        "$<TARGET_BUNDLE_DIR:typenoodle>/Contents/Resources/qml/TypeNoodle/typenoodle.qmltypes"
    COMMENT "Copying TypeNoodle QML module metadata to Resources"
)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(typenoodle PROPERTIES
        WIN32_EXECUTABLE ON
    )
elseif(APPLE)
    # macOS-specific settings
    set_target_properties(typenoodle PROPERTIES
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist
    )
endif()

# Install targets
install(TARGETS typenoodle
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

# Qt deployment
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${Qt6_DIR}/../../../bin)
    add_custom_command(TARGET typenoodle POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR}/qml $<TARGET_FILE:typenoodle>
    )
elseif(APPLE)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS ${Qt6_DIR}/../../../bin)
    add_custom_command(TARGET typenoodle POST_BUILD
        COMMAND ${MACDEPLOYQT_EXECUTABLE} $<TARGET_BUNDLE_DIR:typenoodle> -qmldir=${CMAKE_SOURCE_DIR}/qml
    )
endif()
