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
include mmxlib/CMakeFiles/mmx.dir/depend.make

# Include the progress variables for this target.
include mmxlib/CMakeFiles/mmx.dir/progress.make

# Include the compile flags for this target's objects.
include mmxlib/CMakeFiles/mmx.dir/flags.make

mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o: mmxlib/CMakeFiles/mmx.dir/flags.make
mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/logs/log.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmx.dir/logs/log.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/logs/log.cpp

mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmx.dir/logs/log.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/logs/log.cpp > CMakeFiles/mmx.dir/logs/log.cpp.i

mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmx.dir/logs/log.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/logs/log.cpp -o CMakeFiles/mmx.dir/logs/log.cpp.s

mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o.requires:

.PHONY : mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o.requires

mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o.provides: mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o.requires
	$(MAKE) -f mmxlib/CMakeFiles/mmx.dir/build.make mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o.provides.build
.PHONY : mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o.provides

mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o.provides.build: mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o


mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o: mmxlib/CMakeFiles/mmx.dir/flags.make
mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/create_sock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmx.dir/net/create_sock.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/create_sock.cpp

mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmx.dir/net/create_sock.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/create_sock.cpp > CMakeFiles/mmx.dir/net/create_sock.cpp.i

mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmx.dir/net/create_sock.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/create_sock.cpp -o CMakeFiles/mmx.dir/net/create_sock.cpp.s

mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o.requires:

.PHONY : mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o.requires

mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o.provides: mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o.requires
	$(MAKE) -f mmxlib/CMakeFiles/mmx.dir/build.make mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o.provides.build
.PHONY : mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o.provides

mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o.provides.build: mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o


mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o: mmxlib/CMakeFiles/mmx.dir/flags.make
mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/write_sock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmx.dir/net/write_sock.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/write_sock.cpp

mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmx.dir/net/write_sock.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/write_sock.cpp > CMakeFiles/mmx.dir/net/write_sock.cpp.i

mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmx.dir/net/write_sock.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/write_sock.cpp -o CMakeFiles/mmx.dir/net/write_sock.cpp.s

mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o.requires:

.PHONY : mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o.requires

mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o.provides: mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o.requires
	$(MAKE) -f mmxlib/CMakeFiles/mmx.dir/build.make mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o.provides.build
.PHONY : mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o.provides

mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o.provides.build: mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o


mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o: mmxlib/CMakeFiles/mmx.dir/flags.make
mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/read_sock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmx.dir/net/read_sock.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/read_sock.cpp

mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmx.dir/net/read_sock.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/read_sock.cpp > CMakeFiles/mmx.dir/net/read_sock.cpp.i

mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmx.dir/net/read_sock.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/read_sock.cpp -o CMakeFiles/mmx.dir/net/read_sock.cpp.s

mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o.requires:

.PHONY : mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o.requires

mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o.provides: mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o.requires
	$(MAKE) -f mmxlib/CMakeFiles/mmx.dir/build.make mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o.provides.build
.PHONY : mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o.provides

mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o.provides.build: mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o


mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o: mmxlib/CMakeFiles/mmx.dir/flags.make
mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/sock_opt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmx.dir/net/sock_opt.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/sock_opt.cpp

mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmx.dir/net/sock_opt.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/sock_opt.cpp > CMakeFiles/mmx.dir/net/sock_opt.cpp.i

mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmx.dir/net/sock_opt.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/sock_opt.cpp -o CMakeFiles/mmx.dir/net/sock_opt.cpp.s

mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o.requires:

.PHONY : mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o.requires

mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o.provides: mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o.requires
	$(MAKE) -f mmxlib/CMakeFiles/mmx.dir/build.make mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o.provides.build
.PHONY : mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o.provides

mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o.provides.build: mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o


mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o: mmxlib/CMakeFiles/mmx.dir/flags.make
mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o: /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/close_sock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vkurbatov/work/mmxprj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mmx.dir/net/close_sock.cpp.o -c /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/close_sock.cpp

mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mmx.dir/net/close_sock.cpp.i"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/close_sock.cpp > CMakeFiles/mmx.dir/net/close_sock.cpp.i

mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mmx.dir/net/close_sock.cpp.s"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/net/close_sock.cpp -o CMakeFiles/mmx.dir/net/close_sock.cpp.s

mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o.requires:

.PHONY : mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o.requires

mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o.provides: mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o.requires
	$(MAKE) -f mmxlib/CMakeFiles/mmx.dir/build.make mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o.provides.build
.PHONY : mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o.provides

mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o.provides.build: mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o


# Object files for target mmx
mmx_OBJECTS = \
"CMakeFiles/mmx.dir/logs/log.cpp.o" \
"CMakeFiles/mmx.dir/net/create_sock.cpp.o" \
"CMakeFiles/mmx.dir/net/write_sock.cpp.o" \
"CMakeFiles/mmx.dir/net/read_sock.cpp.o" \
"CMakeFiles/mmx.dir/net/sock_opt.cpp.o" \
"CMakeFiles/mmx.dir/net/close_sock.cpp.o"

# External object files for target mmx
mmx_EXTERNAL_OBJECTS =

/home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a: mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a: mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a: mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a: mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a: mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a: mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o
/home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a: mmxlib/CMakeFiles/mmx.dir/build.make
/home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a: mmxlib/CMakeFiles/mmx.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vkurbatov/work/mmxprj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a"
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && $(CMAKE_COMMAND) -P CMakeFiles/mmx.dir/cmake_clean_target.cmake
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mmx.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mmxlib/CMakeFiles/mmx.dir/build: /home/vkurbatov/work/mmxprj/mmxprj/mmxlib/build/DEBUG/libmmx.a

.PHONY : mmxlib/CMakeFiles/mmx.dir/build

mmxlib/CMakeFiles/mmx.dir/requires: mmxlib/CMakeFiles/mmx.dir/logs/log.cpp.o.requires
mmxlib/CMakeFiles/mmx.dir/requires: mmxlib/CMakeFiles/mmx.dir/net/create_sock.cpp.o.requires
mmxlib/CMakeFiles/mmx.dir/requires: mmxlib/CMakeFiles/mmx.dir/net/write_sock.cpp.o.requires
mmxlib/CMakeFiles/mmx.dir/requires: mmxlib/CMakeFiles/mmx.dir/net/read_sock.cpp.o.requires
mmxlib/CMakeFiles/mmx.dir/requires: mmxlib/CMakeFiles/mmx.dir/net/sock_opt.cpp.o.requires
mmxlib/CMakeFiles/mmx.dir/requires: mmxlib/CMakeFiles/mmx.dir/net/close_sock.cpp.o.requires

.PHONY : mmxlib/CMakeFiles/mmx.dir/requires

mmxlib/CMakeFiles/mmx.dir/clean:
	cd /home/vkurbatov/work/mmxprj/build/mmxlib && $(CMAKE_COMMAND) -P CMakeFiles/mmx.dir/cmake_clean.cmake
.PHONY : mmxlib/CMakeFiles/mmx.dir/clean

mmxlib/CMakeFiles/mmx.dir/depend:
	cd /home/vkurbatov/work/mmxprj/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vkurbatov/work/mmxprj/mmxprj /home/vkurbatov/work/mmxprj/mmxprj/mmxlib /home/vkurbatov/work/mmxprj/build /home/vkurbatov/work/mmxprj/build/mmxlib /home/vkurbatov/work/mmxprj/build/mmxlib/CMakeFiles/mmx.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mmxlib/CMakeFiles/mmx.dir/depend

