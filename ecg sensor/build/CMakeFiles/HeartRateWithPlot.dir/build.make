# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/Desktop/I2C05

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Desktop/I2C05/build

# Include any dependencies generated for this target.
include CMakeFiles/HeartRateWithPlot.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/HeartRateWithPlot.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/HeartRateWithPlot.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HeartRateWithPlot.dir/flags.make

CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.o: CMakeFiles/HeartRateWithPlot.dir/flags.make
CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.o: HeartRateWithPlot_autogen/mocs_compilation.cpp
CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.o: CMakeFiles/HeartRateWithPlot.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pi/Desktop/I2C05/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.o -MF CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.o -c /home/pi/Desktop/I2C05/build/HeartRateWithPlot_autogen/mocs_compilation.cpp

CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Desktop/I2C05/build/HeartRateWithPlot_autogen/mocs_compilation.cpp > CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.i

CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Desktop/I2C05/build/HeartRateWithPlot_autogen/mocs_compilation.cpp -o CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.s

CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.o: CMakeFiles/HeartRateWithPlot.dir/flags.make
CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.o: /home/pi/Desktop/I2C05/main_threaded_with_plot.cpp
CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.o: CMakeFiles/HeartRateWithPlot.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pi/Desktop/I2C05/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.o -MF CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.o.d -o CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.o -c /home/pi/Desktop/I2C05/main_threaded_with_plot.cpp

CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Desktop/I2C05/main_threaded_with_plot.cpp > CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.i

CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Desktop/I2C05/main_threaded_with_plot.cpp -o CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.s

CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.o: CMakeFiles/HeartRateWithPlot.dir/flags.make
CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.o: /home/pi/Desktop/I2C05/mainwindow_threaded_with_plot.cpp
CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.o: CMakeFiles/HeartRateWithPlot.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pi/Desktop/I2C05/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.o -MF CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.o.d -o CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.o -c /home/pi/Desktop/I2C05/mainwindow_threaded_with_plot.cpp

CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Desktop/I2C05/mainwindow_threaded_with_plot.cpp > CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.i

CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Desktop/I2C05/mainwindow_threaded_with_plot.cpp -o CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.s

# Object files for target HeartRateWithPlot
HeartRateWithPlot_OBJECTS = \
"CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.o" \
"CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.o"

# External object files for target HeartRateWithPlot
HeartRateWithPlot_EXTERNAL_OBJECTS =

HeartRateWithPlot: CMakeFiles/HeartRateWithPlot.dir/HeartRateWithPlot_autogen/mocs_compilation.cpp.o
HeartRateWithPlot: CMakeFiles/HeartRateWithPlot.dir/main_threaded_with_plot.cpp.o
HeartRateWithPlot: CMakeFiles/HeartRateWithPlot.dir/mainwindow_threaded_with_plot.cpp.o
HeartRateWithPlot: CMakeFiles/HeartRateWithPlot.dir/build.make
HeartRateWithPlot: /usr/lib/aarch64-linux-gnu/libQt5Charts.so.5.15.8
HeartRateWithPlot: /usr/lib/aarch64-linux-gnu/libQt5Widgets.so.5.15.8
HeartRateWithPlot: /usr/lib/aarch64-linux-gnu/libQt5Gui.so.5.15.8
HeartRateWithPlot: /usr/lib/aarch64-linux-gnu/libQt5Core.so.5.15.8
HeartRateWithPlot: CMakeFiles/HeartRateWithPlot.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/pi/Desktop/I2C05/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable HeartRateWithPlot"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HeartRateWithPlot.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HeartRateWithPlot.dir/build: HeartRateWithPlot
.PHONY : CMakeFiles/HeartRateWithPlot.dir/build

CMakeFiles/HeartRateWithPlot.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HeartRateWithPlot.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HeartRateWithPlot.dir/clean

CMakeFiles/HeartRateWithPlot.dir/depend:
	cd /home/pi/Desktop/I2C05/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Desktop/I2C05 /home/pi/Desktop/I2C05 /home/pi/Desktop/I2C05/build /home/pi/Desktop/I2C05/build /home/pi/Desktop/I2C05/build/CMakeFiles/HeartRateWithPlot.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/HeartRateWithPlot.dir/depend

