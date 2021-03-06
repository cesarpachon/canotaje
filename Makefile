# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cesar/sena_canotaje/canotaje01

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cesar/sena_canotaje/canotaje01

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running interactive CMake command-line interface..."
	/usr/bin/cmake -i .
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/cesar/sena_canotaje/canotaje01/CMakeFiles /home/cesar/sena_canotaje/canotaje01/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/cesar/sena_canotaje/canotaje01/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named CopyConfig

# Build rule for target.
CopyConfig: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 CopyConfig
.PHONY : CopyConfig

# fast build rule for target.
CopyConfig/fast:
	$(MAKE) -f CMakeFiles/CopyConfig.dir/build.make CMakeFiles/CopyConfig.dir/build
.PHONY : CopyConfig/fast

#=============================================================================
# Target rules for targets named CopyRes

# Build rule for target.
CopyRes: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 CopyRes
.PHONY : CopyRes

# fast build rule for target.
CopyRes/fast:
	$(MAKE) -f CMakeFiles/CopyRes.dir/build.make CMakeFiles/CopyRes.dir/build
.PHONY : CopyRes/fast

#=============================================================================
# Target rules for targets named canotaje01

# Build rule for target.
canotaje01: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 canotaje01
.PHONY : canotaje01

# fast build rule for target.
canotaje01/fast:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/build
.PHONY : canotaje01/fast

src/Principal.o: src/Principal.cpp.o
.PHONY : src/Principal.o

# target to build an object file
src/Principal.cpp.o:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/Principal.cpp.o
.PHONY : src/Principal.cpp.o

src/Principal.i: src/Principal.cpp.i
.PHONY : src/Principal.i

# target to preprocess a source file
src/Principal.cpp.i:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/Principal.cpp.i
.PHONY : src/Principal.cpp.i

src/Principal.s: src/Principal.cpp.s
.PHONY : src/Principal.s

# target to generate assembly for a file
src/Principal.cpp.s:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/Principal.cpp.s
.PHONY : src/Principal.cpp.s

src/boat.o: src/boat.cpp.o
.PHONY : src/boat.o

# target to build an object file
src/boat.cpp.o:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/boat.cpp.o
.PHONY : src/boat.cpp.o

src/boat.i: src/boat.cpp.i
.PHONY : src/boat.i

# target to preprocess a source file
src/boat.cpp.i:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/boat.cpp.i
.PHONY : src/boat.cpp.i

src/boat.s: src/boat.cpp.s
.PHONY : src/boat.s

# target to generate assembly for a file
src/boat.cpp.s:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/boat.cpp.s
.PHONY : src/boat.cpp.s

src/context.o: src/context.cpp.o
.PHONY : src/context.o

# target to build an object file
src/context.cpp.o:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/context.cpp.o
.PHONY : src/context.cpp.o

src/context.i: src/context.cpp.i
.PHONY : src/context.i

# target to preprocess a source file
src/context.cpp.i:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/context.cpp.i
.PHONY : src/context.cpp.i

src/context.s: src/context.cpp.s
.PHONY : src/context.s

# target to generate assembly for a file
src/context.cpp.s:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/context.cpp.s
.PHONY : src/context.cpp.s

src/oardevice.o: src/oardevice.cpp.o
.PHONY : src/oardevice.o

# target to build an object file
src/oardevice.cpp.o:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/oardevice.cpp.o
.PHONY : src/oardevice.cpp.o

src/oardevice.i: src/oardevice.cpp.i
.PHONY : src/oardevice.i

# target to preprocess a source file
src/oardevice.cpp.i:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/oardevice.cpp.i
.PHONY : src/oardevice.cpp.i

src/oardevice.s: src/oardevice.cpp.s
.PHONY : src/oardevice.s

# target to generate assembly for a file
src/oardevice.cpp.s:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/oardevice.cpp.s
.PHONY : src/oardevice.cpp.s

src/obplineset.o: src/obplineset.cpp.o
.PHONY : src/obplineset.o

# target to build an object file
src/obplineset.cpp.o:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obplineset.cpp.o
.PHONY : src/obplineset.cpp.o

src/obplineset.i: src/obplineset.cpp.i
.PHONY : src/obplineset.i

# target to preprocess a source file
src/obplineset.cpp.i:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obplineset.cpp.i
.PHONY : src/obplineset.cpp.i

src/obplineset.s: src/obplineset.cpp.s
.PHONY : src/obplineset.s

# target to generate assembly for a file
src/obplineset.cpp.s:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obplineset.cpp.s
.PHONY : src/obplineset.cpp.s

src/obpmathutil.o: src/obpmathutil.cpp.o
.PHONY : src/obpmathutil.o

# target to build an object file
src/obpmathutil.cpp.o:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obpmathutil.cpp.o
.PHONY : src/obpmathutil.cpp.o

src/obpmathutil.i: src/obpmathutil.cpp.i
.PHONY : src/obpmathutil.i

# target to preprocess a source file
src/obpmathutil.cpp.i:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obpmathutil.cpp.i
.PHONY : src/obpmathutil.cpp.i

src/obpmathutil.s: src/obpmathutil.cpp.s
.PHONY : src/obpmathutil.s

# target to generate assembly for a file
src/obpmathutil.cpp.s:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obpmathutil.cpp.s
.PHONY : src/obpmathutil.cpp.s

src/obppulse.o: src/obppulse.cpp.o
.PHONY : src/obppulse.o

# target to build an object file
src/obppulse.cpp.o:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obppulse.cpp.o
.PHONY : src/obppulse.cpp.o

src/obppulse.i: src/obppulse.cpp.i
.PHONY : src/obppulse.i

# target to preprocess a source file
src/obppulse.cpp.i:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obppulse.cpp.i
.PHONY : src/obppulse.cpp.i

src/obppulse.s: src/obppulse.cpp.s
.PHONY : src/obppulse.s

# target to generate assembly for a file
src/obppulse.cpp.s:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obppulse.cpp.s
.PHONY : src/obppulse.cpp.s

src/obpstringutil.o: src/obpstringutil.cpp.o
.PHONY : src/obpstringutil.o

# target to build an object file
src/obpstringutil.cpp.o:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obpstringutil.cpp.o
.PHONY : src/obpstringutil.cpp.o

src/obpstringutil.i: src/obpstringutil.cpp.i
.PHONY : src/obpstringutil.i

# target to preprocess a source file
src/obpstringutil.cpp.i:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obpstringutil.cpp.i
.PHONY : src/obpstringutil.cpp.i

src/obpstringutil.s: src/obpstringutil.cpp.s
.PHONY : src/obpstringutil.s

# target to generate assembly for a file
src/obpstringutil.cpp.s:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/obpstringutil.cpp.s
.PHONY : src/obpstringutil.cpp.s

src/river.o: src/river.cpp.o
.PHONY : src/river.o

# target to build an object file
src/river.cpp.o:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/river.cpp.o
.PHONY : src/river.cpp.o

src/river.i: src/river.cpp.i
.PHONY : src/river.i

# target to preprocess a source file
src/river.cpp.i:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/river.cpp.i
.PHONY : src/river.cpp.i

src/river.s: src/river.cpp.s
.PHONY : src/river.s

# target to generate assembly for a file
src/river.cpp.s:
	$(MAKE) -f CMakeFiles/canotaje01.dir/build.make CMakeFiles/canotaje01.dir/src/river.cpp.s
.PHONY : src/river.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... CopyConfig"
	@echo "... CopyRes"
	@echo "... canotaje01"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... src/Principal.o"
	@echo "... src/Principal.i"
	@echo "... src/Principal.s"
	@echo "... src/boat.o"
	@echo "... src/boat.i"
	@echo "... src/boat.s"
	@echo "... src/context.o"
	@echo "... src/context.i"
	@echo "... src/context.s"
	@echo "... src/oardevice.o"
	@echo "... src/oardevice.i"
	@echo "... src/oardevice.s"
	@echo "... src/obplineset.o"
	@echo "... src/obplineset.i"
	@echo "... src/obplineset.s"
	@echo "... src/obpmathutil.o"
	@echo "... src/obpmathutil.i"
	@echo "... src/obpmathutil.s"
	@echo "... src/obppulse.o"
	@echo "... src/obppulse.i"
	@echo "... src/obppulse.s"
	@echo "... src/obpstringutil.o"
	@echo "... src/obpstringutil.i"
	@echo "... src/obpstringutil.s"
	@echo "... src/river.o"
	@echo "... src/river.i"
	@echo "... src/river.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

