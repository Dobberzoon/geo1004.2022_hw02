# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cjdemo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cjdemo.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cjdemo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cjdemo.dir/flags.make

CMakeFiles/cjdemo.dir/src/main.cpp.o: CMakeFiles/cjdemo.dir/flags.make
CMakeFiles/cjdemo.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/cjdemo.dir/src/main.cpp.o: CMakeFiles/cjdemo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cjdemo.dir/src/main.cpp.o"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cjdemo.dir/src/main.cpp.o -MF CMakeFiles/cjdemo.dir/src/main.cpp.o.d -o CMakeFiles/cjdemo.dir/src/main.cpp.o -c /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp/src/main.cpp

CMakeFiles/cjdemo.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cjdemo.dir/src/main.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp/src/main.cpp > CMakeFiles/cjdemo.dir/src/main.cpp.i

CMakeFiles/cjdemo.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cjdemo.dir/src/main.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp/src/main.cpp -o CMakeFiles/cjdemo.dir/src/main.cpp.s

# Object files for target cjdemo
cjdemo_OBJECTS = \
"CMakeFiles/cjdemo.dir/src/main.cpp.o"

# External object files for target cjdemo
cjdemo_EXTERNAL_OBJECTS =

cjdemo: CMakeFiles/cjdemo.dir/src/main.cpp.o
cjdemo: CMakeFiles/cjdemo.dir/build.make
cjdemo: CMakeFiles/cjdemo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cjdemo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cjdemo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cjdemo.dir/build: cjdemo
.PHONY : CMakeFiles/cjdemo.dir/build

CMakeFiles/cjdemo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cjdemo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cjdemo.dir/clean

CMakeFiles/cjdemo.dir/depend:
	cd /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp/cmake-build-debug /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp/cmake-build-debug /Users/leokan/Documents/TUDelft/GEO1004-2022/3d-hw/hw2/geo1004.2022_hw02/hw/02/cpp/cmake-build-debug/CMakeFiles/cjdemo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cjdemo.dir/depend

