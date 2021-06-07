cmake_minimum_required(VERSION 3.16.3)

set(this INIEditorTests)

set(Sources
  
  INIEditorTests.cpp
)

add_executable(${this} ${Sources})

target_link_libraries(${this} PUBLIC
  gtest_main 
  INIEditor_test
)

add_test(
  NAME ${this}
  COMMAND ${this}
)