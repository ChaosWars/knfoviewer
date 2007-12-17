ADD_SUBDIRECTORY( icons )

IF( COMPILE_FOR_KDE4 )

INCLUDE_DIRECTORIES(${KDE4_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR} )

KDE4_ADD_UI_FILES( KPART_UI_SRC color4.ui font4.ui )
SET( SRC main.cpp knfoviewer.cpp )
SET( KPART_SRC knfoviewer_part.cpp mainwidget.cpp knfoviewerhtml.cpp cp437codec.cpp settings.cpp )
SET( THUMBNAIL_SRC knfoviewerthumbnail.cpp )
KDE4_ADD_KCFG_FILES( KPART_SRC knfoviewersettings.kcfgc )
ADD_DEFINITIONS( ${QT4_DEFINITIONS} ${KDE3_DEFINITIONS} )
LINK_DIRECTORIES( ${KDE4_LIB_DIR} )
KDE4_AUTOMOC( ${KPART_SRC} ${THUMBNAIL_SRC} ${SRC} )
KDE4_ADD_PLUGIN( knfoviewerpart WITH_PREFIX ${KPART_SRC} ${KPART_UI_SRC} )
KDE3_ADD_PLUGIN( knfoviewerthumbnail ${THUMBNAIL_SRC} )
KDE4_ADD_EXECUTABLE( knfoviewer ${SRC} )
TARGET_LINK_LIBRARIES( knfoviewerpart ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KPART_LIBS} ${KDE4_KHTML_LIBS} )
TARGET_LINK_LIBRARIES( knfoviewerthumbnail ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KPART_LIBS} ${KDE4_KHTML_LIBS} )
TARGET_LINK_LIBRARIES( knfoviewer ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KHTML_LIBS} )
INSTALL(TARGETS knfoviewer knfoviewerpart knfoviewerthumbnail
        RUNTIME DESTINATION ${KDE4_BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${PLUGIN_INSTALL_DIR} )
INSTALL( FILES knfoviewer.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde4 )
INSTALL( FILES knfoviewer_part.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
INSTALL( FILES x-nfo.desktop DESTINATION ${MIME_INSTALL_DIR} )
INSTALL( FILES knfoviewerthumbnail.desktop DESTINATION ${${SERVICES_INSTALL_DIR}} )
INSTALL( FILES knfoviewer_shell.rc knfoviewer_part.rc DESTINATION ${DATA_INSTALL_DIR}/knfoviewer )

ELSEIF( COMPILE_FOR_KDE4 )

INCLUDE_DIRECTORIES(${KDE3_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR} )

IF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    SET( CMAKE_INSTALL_PREFIX ${KDE3PREFIX} CACHE PATH "knfoviewer install prefix" FORCE )
ENDIF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )

KDE3_ADD_UI_FILES( KPART_UI_SRC color.ui )
SET( SRC main.cpp knfoviewer.cpp )
SET( KPART_SRC knfoviewer_part.cpp mainwidget.cpp knfoviewerhtml.cpp cp437codec.cpp settings.cpp )
SET( THUMBNAIL_SRC knfoviewerthumbnail.cpp )
KDE3_ADD_KCFG_FILES( KPART_SRC knfoviewersettings.kcfgc )
ADD_DEFINITIONS( ${QT_DEFINITIONS} ${KDE3_DEFINITIONS} )
LINK_DIRECTORIES( ${KDE3_LIB_DIR} )
KDE3_AUTOMOC( ${KPART_SRC} ${THUMBNAIL_SRC} ${SRC} )
KDE3_ADD_KPART( knfoviewerpart WITH_PREFIX ${KPART_SRC} ${KPART_UI_SRC} )
KDE3_ADD_KPART( knfoviewerthumbnail ${THUMBNAIL_SRC} )
KDE3_ADD_EXECUTABLE( knfoviewer ${SRC} )
TARGET_LINK_LIBRARIES( knfoviewerpart ${QT_AND_KDECORE_LIBS} kio kparts khtml )
TARGET_LINK_LIBRARIES( knfoviewerthumbnail ${QT_AND_KDECORE_LIBS} kio kparts khtml )
TARGET_LINK_LIBRARIES( knfoviewer ${QT_AND_KDECORE_LIBS} kdeui kio kparts )
INSTALL(TARGETS knfoviewer knfoviewerpart knfoviewerthumbnail
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/kde3
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/kde3 )
INSTALL( FILES knfoviewer.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde )
INSTALL( FILES knfoviewer_part.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/services )
INSTALL( FILES x-nfo.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/mimelnk/application )
INSTALL( FILES knfoviewerthumbnail.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/services )
INSTALL( FILES knfoviewer_shell.rc knfoviewer_part.rc DESTINATION ${CMAKE_INSTALL_PREFIX}/share/apps/knfoviewer )

ENDIF( COMPILE_FOR_KDE4 )
