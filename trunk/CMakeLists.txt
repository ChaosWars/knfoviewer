PROJECT(knfoviewer)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE OFF)

#So that cmake can find local cmake files
SET( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

#list all source files here
ADD_SUBDIRECTORY( src )
ADD_SUBDIRECTORY( docs )
