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
CXXFLAGS = -march=rv64gcv -O3 -Wall

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
