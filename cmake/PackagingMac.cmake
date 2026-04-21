message(STATUS "Configuring macOS packaging")

set(INSTALL_DIR ${CMAKE_BINARY_DIR}/install)

# Application icon
set(APP_ICON_MACOS ${CMAKE_SOURCE_DIR}/icon/${APP_ICON_NAME}.icns)

set_source_files_properties(${APP_ICON_MACOS}
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources"
)

target_sources(${APP_TARGET_NAME} PRIVATE ${APP_ICON_MACOS})

set_target_properties(${APP_TARGET_NAME} PROPERTIES
    MACOSX_BUNDLE_ICON_FILE ${APP_ICON_NAME}.icns
)

install(TARGETS ${APP_TARGET_NAME}
    BUNDLE DESTINATION .
)

install(FILES ${APP_ICON_MACOS}
    DESTINATION ${APP_TARGET_NAME}.app/Contents/Resources
)

add_custom_target(install_local
    COMMAND ${CMAKE_COMMAND}
        --install ${CMAKE_BINARY_DIR}
        --prefix ${INSTALL_DIR}
    DEPENDS ${APP_TARGET_NAME}
)

find_program(MACDEPLOYQT_EXECUTABLE macdeployqt REQUIRED)

add_custom_target(dmg
    COMMAND ${MACDEPLOYQT_EXECUTABLE}
        ${INSTALL_DIR}/${APP_TARGET_NAME}.app
        -dmg
    COMMAND ${CMAKE_COMMAND} -E rename
        ${INSTALL_DIR}/${APP_TARGET_NAME}.dmg
        ${CMAKE_BINARY_DIR}/${APP_TARGET_NAME}-v${PROJECT_VERSION}.dmg
    DEPENDS install_local
)
