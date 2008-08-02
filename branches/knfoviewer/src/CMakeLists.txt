ADD_SUBDIRECTORY( icons )
ADD_SUBDIRECTORY( mimetypes )
INCLUDE_DIRECTORIES(${KDE3_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR} )

IF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    SET( CMAKE_INSTALL_PREFIX ${KDE3PREFIX} CACHE PATH "knfoviewer install prefix" FORCE )
ENDIF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )

SET( SRC main.cpp knfoviewer.cpp )
SET( KPART_SRC knfoviewerpart.cpp
               knfoviewerhtml.cpp
               cp437codec.cpp )
SET( THUMBNAIL_SRC knfoviewerthumbnail.cpp cp437codec.cpp )
KDE3_ADD_UI_FILES( KPART_UI_SRC colorpage.ui fontpage.ui )
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
        LIBRARY DESTINATION ${KDE3_LIB_DIR}/kde3
        ARCHIVE DESTINATION ${KDE3_LIB_DIR}/kde3 )
INSTALL( FILES knfoviewer.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde )
INSTALL( FILES knfoviewerpart.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/services )
INSTALL( FILES knfoviewerthumbnail.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/services )
INSTALL( FILES knfoviewerui.rc knfoviewerpart.rc DESTINATION ${CMAKE_INSTALL_PREFIX}/share/apps/knfoviewer )
