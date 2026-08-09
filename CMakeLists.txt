cmake_minimum_required(VERSION 3.23)

project(MangoHud_OBS VERSION 0.0.1)

find_package(PkgConfig REQUIRED)

include(GNUInstallDirs)

pkg_check_modules(obs_frontend_api obs-frontend-api)
add_library(MangoHud_OBS SHARED obs_plugin.c)
set_target_properties(MangoHud_OBS PROPERTIES PREFIX "")
target_include_directories(MangoHud_OBS PRIVATE
    ${obs_frontend_api_INCLUDE_DIRS}
    /usr/include/obs
)
target_link_libraries(MangoHud_OBS PRIVATE ${obs_frontend_api_LIBRARIES})

install(TARGETS MangoHud_OBS
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/obs-plugins
)

