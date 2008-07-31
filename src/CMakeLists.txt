ADD_SUBDIRECTORY( icons )
INCLUDE_DIRECTORIES(${KDE4_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR} )
SET( SRC main.cpp knfoviewer.cpp )
SET( KPART_SRC knfoviewerpart.cpp
               mainwidget.cpp
               fontpage.cpp
               colorpage.cpp
               knfoviewerhtml.cpp
               cp437codec.cpp )
SET( KPART_UI_SRC colorpage.ui fontpage.ui )
SET( THUMBNAIL_SRC knfoviewerthumbnail.cpp cp437codec.cpp )
KDE4_ADD_UI_FILES( KPART_SRC ${KPART_UI_SRC} )
KDE4_ADD_KCFG_FILES( KPART_SRC knfoviewersettings.kcfgc )
ADD_DEFINITIONS( ${QT_DEFINITIONS} ${KDE4_DEFINITIONS} )
LINK_DIRECTORIES( ${KDE4_LIB_DIR} )
KDE4_ADD_PLUGIN( knfoviewerpart ${KPART_SRC} )
KDE4_ADD_PLUGIN( knfoviewerthumbnail ${THUMBNAIL_SRC} )
KDE4_ADD_EXECUTABLE( knfoviewer ${SRC} )
TARGET_LINK_LIBRARIES( knfoviewerpart ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KPARTS_LIBS} ${KDE4_KHTML_LIBS} )
TARGET_LINK_LIBRARIES( knfoviewerthumbnail ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KPARTS_LIBS} ${KDE4_KHTML_LIBS} )
TARGET_LINK_LIBRARIES( knfoviewer ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} )
INSTALL(TARGETS knfoviewer knfoviewerpart knfoviewerthumbnail
        RUNTIME DESTINATION ${KDE4_BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${PLUGIN_INSTALL_DIR} )
INSTALL( FILES knfoviewer.desktop DESTINATION ${KDE4_XDG_APPS_INSTALL_DIR} )
INSTALL( FILES knfoviewerpart.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
INSTALL( FILES knfoviewerthumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
INSTALL( FILES knfoviewerui.rc knfoviewerpart.rc DESTINATION ${DATA_INSTALL_DIR}/knfoviewer )
