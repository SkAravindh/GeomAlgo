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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build

# Include any dependencies generated for this target.
include codes/CMakeFiles/geom_Algos.dir/depend.make

# Include the progress variables for this target.
include codes/CMakeFiles/geom_Algos.dir/progress.make

# Include the compile flags for this target's objects.
include codes/CMakeFiles/geom_Algos.dir/flags.make

codes/CMakeFiles/geom_Algos.dir/main.cpp.o: codes/CMakeFiles/geom_Algos.dir/flags.make
codes/CMakeFiles/geom_Algos.dir/main.cpp.o: ../codes/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object codes/CMakeFiles/geom_Algos.dir/main.cpp.o"
	cd /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/codes && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/geom_Algos.dir/main.cpp.o -c /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/codes/main.cpp

codes/CMakeFiles/geom_Algos.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/geom_Algos.dir/main.cpp.i"
	cd /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/codes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/codes/main.cpp > CMakeFiles/geom_Algos.dir/main.cpp.i

codes/CMakeFiles/geom_Algos.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/geom_Algos.dir/main.cpp.s"
	cd /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/codes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/codes/main.cpp -o CMakeFiles/geom_Algos.dir/main.cpp.s

# Object files for target geom_Algos
geom_Algos_OBJECTS = \
"CMakeFiles/geom_Algos.dir/main.cpp.o"

# External object files for target geom_Algos
geom_Algos_EXTERNAL_OBJECTS =

codes/geom_Algos: codes/CMakeFiles/geom_Algos.dir/main.cpp.o
codes/geom_Algos: codes/CMakeFiles/geom_Algos.dir/build.make
codes/geom_Algos: codes/libGeom_Algo.a
codes/geom_Algos: codes/CMakeFiles/geom_Algos.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable geom_Algos"
	cd /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/codes && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/geom_Algos.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
codes/CMakeFiles/geom_Algos.dir/build: codes/geom_Algos

.PHONY : codes/CMakeFiles/geom_Algos.dir/build

codes/CMakeFiles/geom_Algos.dir/clean:
	cd /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/codes && $(CMAKE_COMMAND) -P CMakeFiles/geom_Algos.dir/cmake_clean.cmake
.PHONY : codes/CMakeFiles/geom_Algos.dir/clean

codes/CMakeFiles/geom_Algos.dir/depend:
	cd /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/codes /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/codes /home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/codes/CMakeFiles/geom_Algos.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : codes/CMakeFiles/geom_Algos.dir/depend

