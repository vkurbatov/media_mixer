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
CMAKE_SOURCE_DIR = /home/vkurbatov/work/mmxprj/mmxprj

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vkurbatov/work/mmxprj/build

# Include any dependencies generated for this target.
include mmxcore/CMakeFiles/mmxcore.dir/depend.make

# Include the progress variables for this target.
include mmxcore/CMakeFiles/mmxcore.dir/progress.make

# Include the compile flags for this target's objects.
include mmxcore/CMakeFiles/mmxcore.dir/flags.make

mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o: mmxcore/CMakeFiles/mmxcore.dir/flags.make
mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxcore/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/mmxcore && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxcore.dir/main.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxcore/main.cpp

mmxcore/CMakeFiles/mmxcore.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxcore.dir/main.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/mmxcore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxcore/main.cpp > CMakeFiles/mmxcore.dir/main.cpp.i

mmxcore/CMakeFiles/mmxcore.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxcore.dir/main.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/mmxcore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxcore/main.cpp -o CMakeFiles/mmxcore.dir/main.cpp.s

mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o.requires:

.PHONY : mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o.requires

mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o.provides: mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o.requires
	$(MAKE) -f mmxcore/CMakeFiles/mmxcore.dir/build.make mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o.provides.build
.PHONY : mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o.provides

mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o.provides.build: mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o


mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o: mmxcore/CMakeFiles/mmxcore.dir/flags.make
mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxcore/mux.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/mmxcore && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxcore.dir/mux.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxcore/mux.cpp

mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxcore.dir/mux.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/mmxcore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxcore/mux.cpp > CMakeFiles/mmxcore.dir/mux.cpp.i

mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxcore.dir/mux.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/mmxcore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxcore/mux.cpp -o CMakeFiles/mmxcore.dir/mux.cpp.s

mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o.requires:

.PHONY : mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o.requires

mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o.provides: mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o.requires
	$(MAKE) -f mmxcore/CMakeFiles/mmxcore.dir/build.make mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o.provides.build
.PHONY : mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o.provides

mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o.provides.build: mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o


# Object files for target mmxcore
mmxcore_OBJECTS = \
"CMakeFiles/mmxcore.dir/main.cpp.o" \
"CMakeFiles/mmxcore.dir/mux.cpp.o"

# External object files for target mmxcore
mmxcore_EXTERNAL_OBJECTS =

/home/vkurbatov/work/mmxprj/mmxprj/mmxcore/build/DEBUG/mmxcore: mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxcore/build/DEBUG/mmxcore: mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxcore/build/DEBUG/mmxcore: mmxcore/CMakeFiles/mmxcore.dir/build.make
/home/vkurbatov/work/mmxprj/mmxprj/mmxcore/build/DEBUG/mmxcore: /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a
/home/vkurbatov/work/mmxprj/mmxprj/mmxcore/build/DEBUG/mmxcore: mmxcore/CMakeFiles/mmxcore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vkurbatov/work/mmxprj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable /home/vkurbatov/work/mmxprj/mmxprj/mmxcore/build/DEBUG/mmxcore"
	cd /home/vkurbatov/work/mmxprj/build/mmxcore && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mmxcore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mmxcore/CMakeFiles/mmxcore.dir/build: /home/vkurbatov/work/mmxprj/mmxprj/mmxcore/build/DEBUG/mmxcore

.PHONY : mmxcore/CMakeFiles/mmxcore.dir/build

mmxcore/CMakeFiles/mmxcore.dir/requires: mmxcore/CMakeFiles/mmxcore.dir/main.cpp.o.requires
mmxcore/CMakeFiles/mmxcore.dir/requires: mmxcore/CMakeFiles/mmxcore.dir/mux.cpp.o.requires

.PHONY : mmxcore/CMakeFiles/mmxcore.dir/requires

mmxcore/CMakeFiles/mmxcore.dir/clean:
	cd /home/vkurbatov/work/mmxprj/build/mmxcore && $(CMAKE_COMMAND) -P CMakeFiles/mmxcore.dir/cmake_clean.cmake
.PHONY : mmxcore/CMakeFiles/mmxcore.dir/clean

mmxcore/CMakeFiles/mmxcore.dir/depend:
	cd /home/vkurbatov/work/mmxprj/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vkurbatov/work/mmxprj/mmxprj /home/vkurbatov/work/mmxprj/mmxprj/mmxcore /home/vkurbatov/work/mmxprj/build /home/vkurbatov/work/mmxprj/build/mmxcore /home/vkurbatov/work/mmxprj/build/mmxcore/CMakeFiles/mmxcore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mmxcore/CMakeFiles/mmxcore.dir/depend

