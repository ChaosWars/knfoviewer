PROJECT(knfoviewer)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE OFF)

#So that cmake can find local cmake files
SET( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

IF( COMPILE_FOR_KDE4 )

FIND_PACKAGE( KDE3 REQUIRED )

ELSEIF( COMPILE_FOR_KDE4 )

FIND_PACKAGE( KDE4 REQUIRED )

ENDIF( COMPILE_FOR_KDE4 )

#list all source files here
ADD_SUBDIRECTORY( src )
ADD_SUBDIRECTORY( doc )
