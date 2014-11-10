cmake_minimum_required(VERSION 2.4.0)
project(lab3)

## Fixa alla headers ##################################################

FILE(GLOB lab3_HEADERS  ./*.h*)

list(APPEND lab3_ALL_HEADERS 
  ${lab3_HEADERS}
  )

source_group("include" FILES ${lab3_HEADERS})

## Fixa alla source-filer ################################################

FILE(GLOB lab3_SRCS  ./*.cpp)

list(APPEND lab3_ALL_SRCS 
  ${lab3_SRCS}
  )

source_group("source" FILES ${lab3_SRCS})



## Include dirs for att slippa lankar mellan mappar ###################

list(APPEND lab3_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


## Fixar lankar till bibliotek ########################################

list(APPEND lab3_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/../externals
  ${CMAKE_SOURCE_DIR}/../externals/GLEW
  ${CMAKE_SOURCE_DIR}/../externals/GLFW
  )

## Lagger till dem ################################################

include_directories(	
  ${lab3_INCLUDE_DIRS}
  )

#################################################################

link_directories(
  ${CMAKE_SOURCE_DIR}/../externals/GLEW/lib
  ${CMAKE_SOURCE_DIR}/../externals/GLFW/lib
  ) 
  
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/export)

add_executable(lab3 ${lab3_ALL_HEADERS} ${lab3_ALL_SRCS} ${lab3_ALL_SHADERS})

if(WIN32)
  target_link_libraries(lab3 glew32.lib opengl32.lib glfw3.lib)
endif(WIN32)

