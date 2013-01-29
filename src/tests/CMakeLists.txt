cmake_minimum_required (VERSION 2.6)

add_executable (sanitize sanitize.cpp)
target_link_libraries (sanitize gfal_plugin_xrootd)
