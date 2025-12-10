cmake_minimum_required(VERSION 3.21)

project(TypeNoodle VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
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

# Add external dependencies
add_subdirectory(external/spdlog)
add_subdirectory(external/json)

# Option to use FreeType (OFF by default, use Qt's font APIs first)
option(USE_FREETYPE "Use FreeType for font parsing" OFF)
if(USE_FREETYPE)
    add_subdirectory(external/freetype)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/core/FontInfo.cpp
    src/core/FontManager.cpp
    src/core/FontCollection.cpp
    src/core/PlatformFonts.cpp
    src/models/FontListModel.cpp
    src/models/CollectionModel.cpp
    src/utils/Config.cpp
    src/utils/Logger.cpp
)

set(HEADERS
    src/core/FontInfo.h
    src/core/FontManager.h
    src/core/FontCollection.h
    src/core/PlatformFonts.h
    src/models/FontListModel.h
    src/models/CollectionModel.h
    src/utils/Config.h
    src/utils/Logger.h
)

# QML files
set(QML_FILES
    qml/Main.qml
    qml/Theme.qml
    qml/components/FontCard.qml
    qml/components/FontPreviewPane.qml
    qml/components/Sidebar.qml
    qml/components/SearchBar.qml
    qml/components/TopBar.qml
    qml/pages/LibraryPage.qml
    qml/pages/SettingsPage.qml
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
    VERSION 1.0
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
