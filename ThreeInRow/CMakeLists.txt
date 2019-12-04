CMAKE_MINIMUM_REQUIRED(VERSION 3.9.6)
PROJECT(ThreeInRow)

SET(CMAKE_CXX_STANDARD 14)

SET( src_files 
     src/main.cpp
     src/ThreeInRowGame.cpp
)

add_executable( ${PROJECT_NAME} ${src_files} )
#target_link_libraries( ${PROJECT_NAME} )
