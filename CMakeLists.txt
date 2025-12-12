cmake_minimum_required(VERSION 3.21)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-includes")
include(GetVersion)
project(TypeNoodle VERSION ${CURRENT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# for auto version.h gen
set (PROJECT_VERSION ${CURRENT_VERSION})
set (PROJECT_NAME "TypeNoodle")
string(TIMESTAMP TODAY "%d/%m/%Y")
set (PROJECT_BUILD_DATE ${TODAY})
set (PROJECT_ORG_DOMAIN "typenoodle.app")
set (PROJECT_ORG_NAME "TypeNoodle")

configure_file (src/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h @ONLY)

# Build type defaults to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Quick
    QuickControls2
    Concurrent
)

qt_policy(SET QTP0004 NEW)
# qt_policy(SET QMP0174 )

qt_standard_project_setup(REQUIRES 6.5)

# External dependencies
add_subdirectory(external/freetype)
add_subdirectory(external/spdlog)

# nlohmann_json is header-only, just need to expose includes
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/json/include
)

# Source files
set(SOURCES
    src/main.cpp
    src/core/FontCollection.cpp
    src/core/FontInfo.cpp
    src/core/FontManager.cpp
    src/core/PlatformFonts.cpp
    src/models/CollectionModel.cpp
    src/models/FontListModel.cpp
    src/utils/Config.cpp
    src/utils/Logger.cpp
    src/utils/SettingsManager.cpp
)

set(HEADERS
    src/core/FontCollection.h
    src/core/FontInfo.h
    src/core/FontManager.h
    src/core/PlatformFonts.h
    src/models/CollectionModel.h
    src/models/FontListModel.h
    src/utils/Config.h
    src/utils/Logger.h
    src/utils/SettingsManager.h
)

# Create the executable
qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    resources/resources.qrc
)

# QML modules (set theme as singleton)
set_source_files_properties(qml/Theme.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(${PROJECT_NAME}
    URI TypeNoodle
    VERSION ${CURRENT_VERSION}
    QML_FILES
        qml/Main.qml
        qml/Theme.qml
        qml/components/FontCard.qml
        qml/components/FontPreviewPane.qml
        qml/components/SearchBar.qml
        qml/components/Sidebar.qml
        qml/components/TopBar.qml
        qml/pages/LibraryPage.qml
        qml/pages/SettingsPage.qml
)


# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Concurrent
    freetype
    spdlog::spdlog
    nlohmann_json
)

# Platform-specific settings
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist
        MACOSX_BUNDLE_BUNDLE_NAME "TypeNoodle"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.typenoodle.app"
    )
elseif(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Install rules
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# For Qt deployment
qt_generate_deploy_qml_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    MACOS_BUNDLE_POST_BUILD
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
)
install(SCRIPT ${deploy_script})