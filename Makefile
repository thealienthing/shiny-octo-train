# Project Name
TARGET = Capstone_Project

# Sources
SRC = ./src
CPP_SOURCES = $(SRC)/main.cpp

# Library Locations
LIBDAISY_DIR = ./libDaisy

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
