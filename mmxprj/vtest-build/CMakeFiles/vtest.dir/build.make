# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vkurbatov/work/mmxprj/vtest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vkurbatov/work/mmxprj/vtest-build

# Include any dependencies generated for this target.
include CMakeFiles/vtest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/vtest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vtest.dir/flags.make

CMakeFiles/vtest.dir/main.cpp.o: CMakeFiles/vtest.dir/flags.make
CMakeFiles/vtest.dir/main.cpp.o: /home/vkurbatov/work/mmxprj/vtest/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/vtest-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/vtest.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vtest.dir/main.cpp.o -c /home/vkurbatov/work/mmxprj/vtest/main.cpp

CMakeFiles/vtest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vtest.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/vtest/main.cpp > CMakeFiles/vtest.dir/main.cpp.i

CMakeFiles/vtest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vtest.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/vtest/main.cpp -o CMakeFiles/vtest.dir/main.cpp.s

CMakeFiles/vtest.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/vtest.dir/main.cpp.o.requires

CMakeFiles/vtest.dir/main.cpp.o.provides: CMakeFiles/vtest.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/vtest.dir/build.make CMakeFiles/vtest.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/vtest.dir/main.cpp.o.provides

CMakeFiles/vtest.dir/main.cpp.o.provides.build: CMakeFiles/vtest.dir/main.cpp.o


# Object files for target vtest
vtest_OBJECTS = \
"CMakeFiles/vtest.dir/main.cpp.o"

# External object files for target vtest
vtest_EXTERNAL_OBJECTS =

/home/vkurbatov/work/mmxprj/vtest/build/DEBUG/vtest: CMakeFiles/vtest.dir/main.cpp.o
/home/vkurbatov/work/mmxprj/vtest/build/DEBUG/vtest: CMakeFiles/vtest.dir/build.make
/home/vkurbatov/work/mmxprj/vtest/build/DEBUG/vtest: /home/vkurbatov/work/mmxprj/vtest/libpcm/build/DEBUG/libpcm.a
/home/vkurbatov/work/mmxprj/vtest/build/DEBUG/vtest: /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a
/home/vkurbatov/work/mmxprj/vtest/build/DEBUG/vtest: CMakeFiles/vtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vkurbatov/work/mmxprj/vtest-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/vkurbatov/work/mmxprj/vtest/build/DEBUG/vtest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/vtest.dir/build: /home/vkurbatov/work/mmxprj/vtest/build/DEBUG/vtest

.PHONY : CMakeFiles/vtest.dir/build

CMakeFiles/vtest.dir/requires: CMakeFiles/vtest.dir/main.cpp.o.requires

.PHONY : CMakeFiles/vtest.dir/requires

CMakeFiles/vtest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vtest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vtest.dir/clean

CMakeFiles/vtest.dir/depend:
	cd /home/vkurbatov/work/mmxprj/vtest-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vkurbatov/work/mmxprj/vtest /home/vkurbatov/work/mmxprj/vtest /home/vkurbatov/work/mmxprj/vtest-build /home/vkurbatov/work/mmxprj/vtest-build /home/vkurbatov/work/mmxprj/vtest-build/CMakeFiles/vtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vtest.dir/depend

