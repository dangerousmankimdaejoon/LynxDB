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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/seer/seerdb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/seer/seerdb/build

# Include any dependencies generated for this target.
include CMakeFiles/SeerDB.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/SeerDB.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/SeerDB.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SeerDB.dir/flags.make

CMakeFiles/SeerDB.dir/src/main.c.o: CMakeFiles/SeerDB.dir/flags.make
CMakeFiles/SeerDB.dir/src/main.c.o: /home/seer/seerdb/src/main.c
CMakeFiles/SeerDB.dir/src/main.c.o: CMakeFiles/SeerDB.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/seer/seerdb/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/SeerDB.dir/src/main.c.o"
	/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/SeerDB.dir/src/main.c.o -MF CMakeFiles/SeerDB.dir/src/main.c.o.d -o CMakeFiles/SeerDB.dir/src/main.c.o -c /home/seer/seerdb/src/main.c

CMakeFiles/SeerDB.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SeerDB.dir/src/main.c.i"
	/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/seer/seerdb/src/main.c > CMakeFiles/SeerDB.dir/src/main.c.i

CMakeFiles/SeerDB.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SeerDB.dir/src/main.c.s"
	/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/seer/seerdb/src/main.c -o CMakeFiles/SeerDB.dir/src/main.c.s

# Object files for target SeerDB
SeerDB_OBJECTS = \
"CMakeFiles/SeerDB.dir/src/main.c.o"

# External object files for target SeerDB
SeerDB_EXTERNAL_OBJECTS =

SeerDB: CMakeFiles/SeerDB.dir/src/main.c.o
SeerDB: CMakeFiles/SeerDB.dir/build.make
SeerDB: CMakeFiles/SeerDB.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/seer/seerdb/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable SeerDB"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SeerDB.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SeerDB.dir/build: SeerDB
.PHONY : CMakeFiles/SeerDB.dir/build

CMakeFiles/SeerDB.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SeerDB.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SeerDB.dir/clean

CMakeFiles/SeerDB.dir/depend:
	cd /home/seer/seerdb/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/seer/seerdb /home/seer/seerdb /home/seer/seerdb/build /home/seer/seerdb/build /home/seer/seerdb/build/CMakeFiles/SeerDB.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SeerDB.dir/depend

