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
CMAKE_SOURCE_DIR = "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi"

# Include any dependencies generated for this target.
include CMakeFiles/tests_get_data.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tests_get_data.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tests_get_data.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tests_get_data.dir/flags.make

CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.o: CMakeFiles/tests_get_data.dir/flags.make
CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.o: src/tests_get_data.cpp
CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.o: CMakeFiles/tests_get_data.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.o -MF CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.o.d -o CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.o -c "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi/src/tests_get_data.cpp"

CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi/src/tests_get_data.cpp" > CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.i

CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi/src/tests_get_data.cpp" -o CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.s

# Object files for target tests_get_data
tests_get_data_OBJECTS = \
"CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.o"

# External object files for target tests_get_data
tests_get_data_EXTERNAL_OBJECTS =

bin/tests_get_data: CMakeFiles/tests_get_data.dir/src/tests_get_data.cpp.o
bin/tests_get_data: CMakeFiles/tests_get_data.dir/build.make
bin/tests_get_data: /usr/local/lib/libCatch2Main.a
bin/tests_get_data: lib/libdata.so
bin/tests_get_data: /usr/local/lib/libCatch2.a
bin/tests_get_data: CMakeFiles/tests_get_data.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/tests_get_data"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tests_get_data.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tests_get_data.dir/build: bin/tests_get_data
.PHONY : CMakeFiles/tests_get_data.dir/build

CMakeFiles/tests_get_data.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tests_get_data.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tests_get_data.dir/clean

CMakeFiles/tests_get_data.dir/depend:
	cd "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi" "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi" "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi" "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi" "/home/pg19/Coventry University/7166CEM-Coursework_Prathmesh_Gardi/CMakeFiles/tests_get_data.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/tests_get_data.dir/depend

