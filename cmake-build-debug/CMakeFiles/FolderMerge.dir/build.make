# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\software\clion\install\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\software\clion\install\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Git\FolderMerge

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Git\FolderMerge\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/FolderMerge.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FolderMerge.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FolderMerge.dir/flags.make

CMakeFiles/FolderMerge.dir/main.cpp.obj: CMakeFiles/FolderMerge.dir/flags.make
CMakeFiles/FolderMerge.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Git\FolderMerge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FolderMerge.dir/main.cpp.obj"
	D:\software\msys2\install\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\FolderMerge.dir\main.cpp.obj -c D:\Git\FolderMerge\main.cpp

CMakeFiles/FolderMerge.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FolderMerge.dir/main.cpp.i"
	D:\software\msys2\install\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Git\FolderMerge\main.cpp > CMakeFiles\FolderMerge.dir\main.cpp.i

CMakeFiles/FolderMerge.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FolderMerge.dir/main.cpp.s"
	D:\software\msys2\install\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Git\FolderMerge\main.cpp -o CMakeFiles\FolderMerge.dir\main.cpp.s

# Object files for target FolderMerge
FolderMerge_OBJECTS = \
"CMakeFiles/FolderMerge.dir/main.cpp.obj"

# External object files for target FolderMerge
FolderMerge_EXTERNAL_OBJECTS =

FolderMerge.exe: CMakeFiles/FolderMerge.dir/main.cpp.obj
FolderMerge.exe: CMakeFiles/FolderMerge.dir/build.make
FolderMerge.exe: CMakeFiles/FolderMerge.dir/linklibs.rsp
FolderMerge.exe: CMakeFiles/FolderMerge.dir/objects1.rsp
FolderMerge.exe: CMakeFiles/FolderMerge.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Git\FolderMerge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable FolderMerge.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\FolderMerge.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FolderMerge.dir/build: FolderMerge.exe

.PHONY : CMakeFiles/FolderMerge.dir/build

CMakeFiles/FolderMerge.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\FolderMerge.dir\cmake_clean.cmake
.PHONY : CMakeFiles/FolderMerge.dir/clean

CMakeFiles/FolderMerge.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Git\FolderMerge D:\Git\FolderMerge D:\Git\FolderMerge\cmake-build-debug D:\Git\FolderMerge\cmake-build-debug D:\Git\FolderMerge\cmake-build-debug\CMakeFiles\FolderMerge.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FolderMerge.dir/depend
