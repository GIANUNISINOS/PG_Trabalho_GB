# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/PROJETO_PG_GA.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/PROJETO_PG_GA.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PROJETO_PG_GA.dir/flags.make

CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.o: CMakeFiles/PROJETO_PG_GA.dir/flags.make
CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.o: ../source/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.o -c /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB/source/main.cpp

CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB/source/main.cpp > CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.i

CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB/source/main.cpp -o CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.s

# Object files for target PROJETO_PG_GA
PROJETO_PG_GA_OBJECTS = \
"CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.o"

# External object files for target PROJETO_PG_GA
PROJETO_PG_GA_EXTERNAL_OBJECTS =

PROJETO_PG_GA: CMakeFiles/PROJETO_PG_GA.dir/source/main.cpp.o
PROJETO_PG_GA: CMakeFiles/PROJETO_PG_GA.dir/build.make
PROJETO_PG_GA: /usr/local/lib/libglfw.3.3.dylib
PROJETO_PG_GA: /usr/local/lib/libGLEW.dylib
PROJETO_PG_GA: CMakeFiles/PROJETO_PG_GA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable PROJETO_PG_GA"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PROJETO_PG_GA.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PROJETO_PG_GA.dir/build: PROJETO_PG_GA

.PHONY : CMakeFiles/PROJETO_PG_GA.dir/build

CMakeFiles/PROJETO_PG_GA.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PROJETO_PG_GA.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PROJETO_PG_GA.dir/clean

CMakeFiles/PROJETO_PG_GA.dir/depend:
	cd /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB/cmake-build-debug /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB/cmake-build-debug /Users/gian/GitHub/PG_Trabalho_GB/PROJETO_PG_GB/cmake-build-debug/CMakeFiles/PROJETO_PG_GA.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PROJETO_PG_GA.dir/depend
