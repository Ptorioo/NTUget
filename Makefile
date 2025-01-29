# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

# Command-line flag to silence $(MAKE).
MAKEFLAGS += -s

# Compiler and build configuration
BUILD_DIR = build

# Detect platform and set generator
ifeq ($(OS),Windows_NT)
    GENERATOR = "MinGW Makefiles"
else
    GENERATOR = "Unix Makefiles"
endif

# Default build type (can override on command line, e.g., make BUILD_TYPE=Release)
BUILD_TYPE ?= Debug

# Target for building the project
.PHONY: all
all: build

# Build target
.PHONY: build
build:
	@echo Building with $(GENERATOR) in $(BUILD_TYPE) mode...
	@cmake -S . -B $(BUILD_DIR) -G $(GENERATOR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	@cmake --build $(BUILD_DIR)

# Clean target to remove build directory
.PHONY: clean
clean:
	@echo Cleaning build directory...
	@cmake -E remove_directory $(BUILD_DIR)

# Help message
define HELP_MESSAGE
Usage:
   make [target] [BUILD_TYPE=<Debug|Release>]
 
Targets:
   build       Build the project (default target)
   clean       Remove the build directory
   help        Show this help message
endef
export HELP_MESSAGE

# Help target to show usage
.PHONY: help
help:
ifeq ($(OS),Windows_NT)
	@echo Usage:
	@echo   make [target] [BUILD_TYPE=^<Debug^|Release^>]
	@echo.
	@echo Targets:
	@echo   build       Build the project (default target)
	@echo   clean       Remove the build directory
	@echo   help        Show this help message
else
	@echo "$$HELP_MESSAGE"
endif
