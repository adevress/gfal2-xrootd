cmake_minimum_required (VERSION 2.6)

project (gfal2-plugin-xrootd)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 2)
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")

include(DefineInstallationPaths REQUIRED)
include(CMakeGeneratePkgConfig REQUIRED)

find_package(PkgConfig REQUIRED)


add_subdirectory (src)

install(FILES "README" "LICENSE" "RELEASE-NOTES"
        DESTINATION ${DOC_INSTALL_DIR})
