# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /home/hr/cmake-3.25.0-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/hr/cmake-3.25.0-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hr/sylar-v2.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hr/sylar-v2.0/build

# Include any dependencies generated for this target.
include CMakeFiles/test_macro.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_macro.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_macro.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_macro.dir/flags.make

CMakeFiles/test_macro.dir/tests/test_macro.cc.o: CMakeFiles/test_macro.dir/flags.make
CMakeFiles/test_macro.dir/tests/test_macro.cc.o: /home/hr/sylar-v2.0/tests/test_macro.cc
CMakeFiles/test_macro.dir/tests/test_macro.cc.o: CMakeFiles/test_macro.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hr/sylar-v2.0/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_macro.dir/tests/test_macro.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_macro.dir/tests/test_macro.cc.o -MF CMakeFiles/test_macro.dir/tests/test_macro.cc.o.d -o CMakeFiles/test_macro.dir/tests/test_macro.cc.o -c /home/hr/sylar-v2.0/tests/test_macro.cc

CMakeFiles/test_macro.dir/tests/test_macro.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_macro.dir/tests/test_macro.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hr/sylar-v2.0/tests/test_macro.cc > CMakeFiles/test_macro.dir/tests/test_macro.cc.i

CMakeFiles/test_macro.dir/tests/test_macro.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_macro.dir/tests/test_macro.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hr/sylar-v2.0/tests/test_macro.cc -o CMakeFiles/test_macro.dir/tests/test_macro.cc.s

# Object files for target test_macro
test_macro_OBJECTS = \
"CMakeFiles/test_macro.dir/tests/test_macro.cc.o"

# External object files for target test_macro
test_macro_EXTERNAL_OBJECTS =

/home/hr/sylar-v2.0/bin/test_macro: CMakeFiles/test_macro.dir/tests/test_macro.cc.o
/home/hr/sylar-v2.0/bin/test_macro: CMakeFiles/test_macro.dir/build.make
/home/hr/sylar-v2.0/bin/test_macro: /home/hr/sylar-v2.0/lib/libsylar.so
/home/hr/sylar-v2.0/bin/test_macro: CMakeFiles/test_macro.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hr/sylar-v2.0/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/hr/sylar-v2.0/bin/test_macro"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_macro.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_macro.dir/build: /home/hr/sylar-v2.0/bin/test_macro
.PHONY : CMakeFiles/test_macro.dir/build

CMakeFiles/test_macro.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_macro.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_macro.dir/clean

CMakeFiles/test_macro.dir/depend:
	cd /home/hr/sylar-v2.0/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hr/sylar-v2.0 /home/hr/sylar-v2.0 /home/hr/sylar-v2.0/build /home/hr/sylar-v2.0/build /home/hr/sylar-v2.0/build/CMakeFiles/test_macro.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_macro.dir/depend
