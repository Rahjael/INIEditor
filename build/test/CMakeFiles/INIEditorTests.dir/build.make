# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rahjael/Coding/INIEditor_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rahjael/Coding/INIEditor_test/build

# Include any dependencies generated for this target.
include test/CMakeFiles/INIEditorTests.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/INIEditorTests.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/INIEditorTests.dir/flags.make

test/CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.o: test/CMakeFiles/INIEditorTests.dir/flags.make
test/CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.o: ../test/INIEditorTests.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rahjael/Coding/INIEditor_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.o"
	cd /home/rahjael/Coding/INIEditor_test/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.o -c /home/rahjael/Coding/INIEditor_test/test/INIEditorTests.cpp

test/CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.i"
	cd /home/rahjael/Coding/INIEditor_test/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rahjael/Coding/INIEditor_test/test/INIEditorTests.cpp > CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.i

test/CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.s"
	cd /home/rahjael/Coding/INIEditor_test/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rahjael/Coding/INIEditor_test/test/INIEditorTests.cpp -o CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.s

# Object files for target INIEditorTests
INIEditorTests_OBJECTS = \
"CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.o"

# External object files for target INIEditorTests
INIEditorTests_EXTERNAL_OBJECTS =

test/INIEditorTests: test/CMakeFiles/INIEditorTests.dir/INIEditorTests.cpp.o
test/INIEditorTests: test/CMakeFiles/INIEditorTests.dir/build.make
test/INIEditorTests: lib/libgtest_maind.a
test/INIEditorTests: libINIEditor_test.a
test/INIEditorTests: lib/libgtestd.a
test/INIEditorTests: test/CMakeFiles/INIEditorTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rahjael/Coding/INIEditor_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable INIEditorTests"
	cd /home/rahjael/Coding/INIEditor_test/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/INIEditorTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/INIEditorTests.dir/build: test/INIEditorTests

.PHONY : test/CMakeFiles/INIEditorTests.dir/build

test/CMakeFiles/INIEditorTests.dir/clean:
	cd /home/rahjael/Coding/INIEditor_test/build/test && $(CMAKE_COMMAND) -P CMakeFiles/INIEditorTests.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/INIEditorTests.dir/clean

test/CMakeFiles/INIEditorTests.dir/depend:
	cd /home/rahjael/Coding/INIEditor_test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rahjael/Coding/INIEditor_test /home/rahjael/Coding/INIEditor_test/test /home/rahjael/Coding/INIEditor_test/build /home/rahjael/Coding/INIEditor_test/build/test /home/rahjael/Coding/INIEditor_test/build/test/CMakeFiles/INIEditorTests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/INIEditorTests.dir/depend

