# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/northernland/Documents/scudb_initial

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/northernland/Documents/scudb_initial/build

# Include any dependencies generated for this target.
include test/CMakeFiles/extendible_hash_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/extendible_hash_test.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/extendible_hash_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/extendible_hash_test.dir/flags.make

test/CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.o: test/CMakeFiles/extendible_hash_test.dir/flags.make
test/CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.o: ../test/hash/extendible_hash_test.cpp
test/CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.o: test/CMakeFiles/extendible_hash_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/northernland/Documents/scudb_initial/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.o"
	cd /home/northernland/Documents/scudb_initial/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.o -MF CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.o.d -o CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.o -c /home/northernland/Documents/scudb_initial/test/hash/extendible_hash_test.cpp

test/CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.i"
	cd /home/northernland/Documents/scudb_initial/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/northernland/Documents/scudb_initial/test/hash/extendible_hash_test.cpp > CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.i

test/CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.s"
	cd /home/northernland/Documents/scudb_initial/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/northernland/Documents/scudb_initial/test/hash/extendible_hash_test.cpp -o CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.s

# Object files for target extendible_hash_test
extendible_hash_test_OBJECTS = \
"CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.o"

# External object files for target extendible_hash_test
extendible_hash_test_EXTERNAL_OBJECTS =

test/extendible_hash_test: test/CMakeFiles/extendible_hash_test.dir/hash/extendible_hash_test.cpp.o
test/extendible_hash_test: test/CMakeFiles/extendible_hash_test.dir/build.make
test/extendible_hash_test: lib/libvtable.so
test/extendible_hash_test: lib/libsqlite3.so
test/extendible_hash_test: lib/libgtest.so
test/extendible_hash_test: test/CMakeFiles/extendible_hash_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/northernland/Documents/scudb_initial/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable extendible_hash_test"
	cd /home/northernland/Documents/scudb_initial/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/extendible_hash_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/extendible_hash_test.dir/build: test/extendible_hash_test
.PHONY : test/CMakeFiles/extendible_hash_test.dir/build

test/CMakeFiles/extendible_hash_test.dir/clean:
	cd /home/northernland/Documents/scudb_initial/build/test && $(CMAKE_COMMAND) -P CMakeFiles/extendible_hash_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/extendible_hash_test.dir/clean

test/CMakeFiles/extendible_hash_test.dir/depend:
	cd /home/northernland/Documents/scudb_initial/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/northernland/Documents/scudb_initial /home/northernland/Documents/scudb_initial/test /home/northernland/Documents/scudb_initial/build /home/northernland/Documents/scudb_initial/build/test /home/northernland/Documents/scudb_initial/build/test/CMakeFiles/extendible_hash_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/extendible_hash_test.dir/depend

