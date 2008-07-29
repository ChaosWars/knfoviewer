PROJECT(knfoviewer)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE OFF)

FIND_PACKAGE( KDE4 REQUIRED )

#list all source files here
ADD_SUBDIRECTORY( src )
ADD_SUBDIRECTORY( doc )
