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
CMAKE_BINARY_DIR = /home/vkurbatov/work/mmxprj/build/DEBUG

# Include any dependencies generated for this target.
include mmxtest/CMakeFiles/mmxtest.dir/depend.make

# Include the progress variables for this target.
include mmxtest/CMakeFiles/mmxtest.dir/progress.make

# Include the compile flags for this target's objects.
include mmxtest/CMakeFiles/mmxtest.dir/flags.make

mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/main.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/main.cpp

mmxtest/CMakeFiles/mmxtest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/main.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/main.cpp > CMakeFiles/mmxtest.dir/main.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/main.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/main.cpp -o CMakeFiles/mmxtest.dir/main.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test1.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test1.cpp

mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test1.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test1.cpp > CMakeFiles/mmxtest.dir/test1.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test1.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test1.cpp -o CMakeFiles/mmxtest.dir/test1.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test2.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test2.cpp

mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test2.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test2.cpp > CMakeFiles/mmxtest.dir/test2.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test2.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test2.cpp -o CMakeFiles/mmxtest.dir/test2.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test3.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test3.cpp

mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test3.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test3.cpp > CMakeFiles/mmxtest.dir/test3.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test3.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test3.cpp -o CMakeFiles/mmxtest.dir/test3.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test4.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test4.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test4.cpp

mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test4.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test4.cpp > CMakeFiles/mmxtest.dir/test4.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test4.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test4.cpp -o CMakeFiles/mmxtest.dir/test4.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test5.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test5.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test5.cpp

mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test5.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test5.cpp > CMakeFiles/mmxtest.dir/test5.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test5.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test5.cpp -o CMakeFiles/mmxtest.dir/test5.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test6.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test6.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test6.cpp

mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test6.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test6.cpp > CMakeFiles/mmxtest.dir/test6.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test6.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test6.cpp -o CMakeFiles/mmxtest.dir/test6.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test7.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test7.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test7.cpp

mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test7.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test7.cpp > CMakeFiles/mmxtest.dir/test7.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test7.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test7.cpp -o CMakeFiles/mmxtest.dir/test7.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test8.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test8.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test8.cpp

mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test8.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test8.cpp > CMakeFiles/mmxtest.dir/test8.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test8.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test8.cpp -o CMakeFiles/mmxtest.dir/test8.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test9.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test9.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test9.cpp

mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test9.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test9.cpp > CMakeFiles/mmxtest.dir/test9.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test9.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test9.cpp -o CMakeFiles/mmxtest.dir/test9.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test10.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test10.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test10.cpp

mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test10.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test10.cpp > CMakeFiles/mmxtest.dir/test10.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test10.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test10.cpp -o CMakeFiles/mmxtest.dir/test10.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o


mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o: mmxtest/CMakeFiles/mmxtest.dir/flags.make
mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test11.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmxtest.dir/test11.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test11.cpp

mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmxtest.dir/test11.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test11.cpp > CMakeFiles/mmxtest.dir/test11.cpp.i

mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmxtest.dir/test11.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/test11.cpp -o CMakeFiles/mmxtest.dir/test11.cpp.s

mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o.requires:

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o.requires

mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o.provides: mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o.requires
	$(MAKE) -f mmxtest/CMakeFiles/mmxtest.dir/build.make mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o.provides.build
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o.provides

mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o.provides.build: mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o


# Object files for target mmxtest
mmxtest_OBJECTS = \
"CMakeFiles/mmxtest.dir/main.cpp.o" \
"CMakeFiles/mmxtest.dir/test1.cpp.o" \
"CMakeFiles/mmxtest.dir/test2.cpp.o" \
"CMakeFiles/mmxtest.dir/test3.cpp.o" \
"CMakeFiles/mmxtest.dir/test4.cpp.o" \
"CMakeFiles/mmxtest.dir/test5.cpp.o" \
"CMakeFiles/mmxtest.dir/test6.cpp.o" \
"CMakeFiles/mmxtest.dir/test7.cpp.o" \
"CMakeFiles/mmxtest.dir/test8.cpp.o" \
"CMakeFiles/mmxtest.dir/test9.cpp.o" \
"CMakeFiles/mmxtest.dir/test10.cpp.o" \
"CMakeFiles/mmxtest.dir/test11.cpp.o"

# External object files for target mmxtest
mmxtest_EXTERNAL_OBJECTS =

/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/build.make
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a
/home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest: mmxtest/CMakeFiles/mmxtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vkurbatov/work/mmxprj/build/DEBUG/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest"
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mmxtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mmxtest/CMakeFiles/mmxtest.dir/build: /home/vkurbatov/work/mmxprj/mmxprj/mmxtest/build/DEBUG/mmxtest

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/build

mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/main.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test1.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test2.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test3.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test4.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test5.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test6.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test7.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test8.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test9.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test10.cpp.o.requires
mmxtest/CMakeFiles/mmxtest.dir/requires: mmxtest/CMakeFiles/mmxtest.dir/test11.cpp.o.requires

.PHONY : mmxtest/CMakeFiles/mmxtest.dir/requires

mmxtest/CMakeFiles/mmxtest.dir/clean:
	cd /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest && $(CMAKE_COMMAND) -P CMakeFiles/mmxtest.dir/cmake_clean.cmake
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/clean

mmxtest/CMakeFiles/mmxtest.dir/depend:
	cd /home/vkurbatov/work/mmxprj/build/DEBUG && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vkurbatov/work/mmxprj/mmxprj /home/vkurbatov/work/mmxprj/mmxprj/mmxtest /home/vkurbatov/work/mmxprj/build/DEBUG /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest /home/vkurbatov/work/mmxprj/build/DEBUG/mmxtest/CMakeFiles/mmxtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mmxtest/CMakeFiles/mmxtest.dir/depend

