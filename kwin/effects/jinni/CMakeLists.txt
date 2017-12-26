cmake_minimum_required(VERSION 3.10)

project(jinieffect)

find_package(ECM 5.38 REQUIRED NO_MODULE)

include(FeatureSummary)
include(WriteBasicConfigVersionFile)
include(GenerateExportHeader)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 CONFIG REQUIRED COMPONENTS
    Core
    Gui
)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMInstallIcons)
include(ECMOptionalAddSubdirectory)

# Set C++ compiler flags.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
endif()

find_package(KF5 REQUIRED COMPONENTS
    Service
    WindowSystem
)

set(jinni_effect_QT_LIBS
    Qt5::Core
    Qt5::Gui
)

set(jinni_effect_KDE_LIBS
    KF5::Service
    KF5::WindowSystem
)

set(jinni_effect_KWIN_LIBS
    kwineffects
)

# Build plugin effect library.
add_library(kwin4_effect_jinni SHARED jinni.cpp)
kcoreaddons_desktop_to_json(kwin4_effect_jinni jinni.desktop SERVICE_TYPES kwineffect.desktop)
target_link_libraries(kwin4_effect_jinni PRIVATE ${jinni_effect_QT_LIBS} ${jinni_effect_KDE_LIBS} ${jinni_effect_KWIN_LIBS})

# Install plugin.
install(
    TARGETS
        kwin4_effect_jinni
    DESTINATION
        ${PLUGIN_INSTALL_DIR}/kwin/effects/plugins/
)
