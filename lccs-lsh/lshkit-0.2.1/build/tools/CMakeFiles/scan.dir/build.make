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


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build

# Include any dependencies generated for this target.
include tools/CMakeFiles/scan.dir/depend.make

# Include the progress variables for this target.
include tools/CMakeFiles/scan.dir/progress.make

# Include the compile flags for this target's objects.
include tools/CMakeFiles/scan.dir/flags.make

tools/CMakeFiles/scan.dir/scan.cpp.o: tools/CMakeFiles/scan.dir/flags.make
tools/CMakeFiles/scan.dir/scan.cpp.o: ../tools/scan.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tools/CMakeFiles/scan.dir/scan.cpp.o"
	cd /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/scan.dir/scan.cpp.o -c /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/tools/scan.cpp

tools/CMakeFiles/scan.dir/scan.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/scan.dir/scan.cpp.i"
	cd /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build/tools && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/tools/scan.cpp > CMakeFiles/scan.dir/scan.cpp.i

tools/CMakeFiles/scan.dir/scan.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/scan.dir/scan.cpp.s"
	cd /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build/tools && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/tools/scan.cpp -o CMakeFiles/scan.dir/scan.cpp.s

# Object files for target scan
scan_OBJECTS = \
"CMakeFiles/scan.dir/scan.cpp.o"

# External object files for target scan
scan_EXTERNAL_OBJECTS =

bin/scan: tools/CMakeFiles/scan.dir/scan.cpp.o
bin/scan: tools/CMakeFiles/scan.dir/build.make
bin/scan: lib/liblshkit.a
bin/scan: /usr/lib/x86_64-linux-gnu/libboost_program_options.a
bin/scan: /usr/lib/x86_64-linux-gnu/libgsl.so
bin/scan: /usr/lib/x86_64-linux-gnu/libgslcblas.so
bin/scan: tools/CMakeFiles/scan.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/scan"
	cd /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build/tools && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/scan.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/CMakeFiles/scan.dir/build: bin/scan

.PHONY : tools/CMakeFiles/scan.dir/build

tools/CMakeFiles/scan.dir/clean:
	cd /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build/tools && $(CMAKE_COMMAND) -P CMakeFiles/scan.dir/cmake_clean.cmake
.PHONY : tools/CMakeFiles/scan.dir/clean

tools/CMakeFiles/scan.dir/depend:
	cd /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1 /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/tools /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build/tools /media/qiang/434bfac8-afaf-4615-85b8-8036532d7f7c/qiang/Projects/lccs-lsh/lshkit-0.2.1/build/tools/CMakeFiles/scan.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/CMakeFiles/scan.dir/depend

