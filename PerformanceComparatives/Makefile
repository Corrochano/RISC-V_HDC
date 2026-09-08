###########################################################################
# Copyright 2026 Álvaro Corrochano López
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###########################################################################
# Compiler and flags
CXX = g++
BASE_FLAGS := -Wall -Wextra

CXXFLAGS ?= $(BASE_FLAGS) -march=rv64gcv -O3

# Files to compile
SRCS = comparation.cpp \
       ../Microbenchmarks/bindBenchmark.cpp \
       ../Microbenchmarks/hammingBenchmark.cpp \
       ../Microbenchmarks/queryBenchmark.cpp \
       ../BasicHDCKernels/scalarKernels.cpp \
       ../BasicHDCKernels/vectorialKernels.cpp

# Object files generated from the source files
OBJS = $(SRCS:.cpp=.o)

# Name of the final executable
TARGET = comparation

# Default target to build the executable
all: $(TARGET)

# --- Flag Profile Targets ---

# RISC-V Vector extension with full O3 optimization
vector-o3: CXXFLAGS := $(BASE_FLAGS) -march=rv64gcv -O3
vector-o3: clean $(TARGET)

# RISC-V Vector extension with O2 optimization
vector-o2: CXXFLAGS := $(BASE_FLAGS) -march=rv64gcv -O2
vector-o2: clean $(TARGET)

# Scalar build without vector extension (rv64g)
scalar: CXXFLAGS := $(BASE_FLAGS) -march=rv64g -O3
scalar: clean $(TARGET)

# Debug build (no optimizations, debug symbols)
debug: CXXFLAGS := $(BASE_FLAGS) -march=rv64gcv -O0 -g -DDEBUG
debug: clean $(TARGET)

# Rule for linking the executable from the .o files
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Rule for compiling each .cpp file individually to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for cleaning generated files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
