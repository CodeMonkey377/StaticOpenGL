# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/josep/coding/StaticOpenGL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/josep/coding/StaticOpenGL/build

# Utility rule file for uninstall.

# Include any custom commands dependencies for this target.
include glfw-3.4/CMakeFiles/uninstall.dir/compiler_depend.make

# Include the progress variables for this target.
include glfw-3.4/CMakeFiles/uninstall.dir/progress.make

glfw-3.4/CMakeFiles/uninstall:
	cd /home/josep/coding/StaticOpenGL/build/glfw-3.4 && /usr/bin/cmake -P /home/josep/coding/StaticOpenGL/build/glfw-3.4/cmake_uninstall.cmake

uninstall: glfw-3.4/CMakeFiles/uninstall
uninstall: glfw-3.4/CMakeFiles/uninstall.dir/build.make
.PHONY : uninstall

# Rule to build all files generated by this target.
glfw-3.4/CMakeFiles/uninstall.dir/build: uninstall
.PHONY : glfw-3.4/CMakeFiles/uninstall.dir/build

glfw-3.4/CMakeFiles/uninstall.dir/clean:
	cd /home/josep/coding/StaticOpenGL/build/glfw-3.4 && $(CMAKE_COMMAND) -P CMakeFiles/uninstall.dir/cmake_clean.cmake
.PHONY : glfw-3.4/CMakeFiles/uninstall.dir/clean

glfw-3.4/CMakeFiles/uninstall.dir/depend:
	cd /home/josep/coding/StaticOpenGL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/josep/coding/StaticOpenGL /home/josep/coding/StaticOpenGL/glfw-3.4 /home/josep/coding/StaticOpenGL/build /home/josep/coding/StaticOpenGL/build/glfw-3.4 /home/josep/coding/StaticOpenGL/build/glfw-3.4/CMakeFiles/uninstall.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : glfw-3.4/CMakeFiles/uninstall.dir/depend

