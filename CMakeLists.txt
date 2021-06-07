cmake_minimum_required(VERSION 3.16.3)

set(this INIEditor)

project(${this} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(googletest)

set(Headers
  src/INIEditor.h
  src/UI.h
  )

set(Sources 
  src/main.cpp 
  
  src/INIEditor.cpp
  src/UI.cpp 
  )



add_library(INIEditor_test STATIC ${Sources} ${Headers})

add_subdirectory(test)

add_executable(${this} ${Sources})

