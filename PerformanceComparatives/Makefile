# Compilador y flags
CXX = g++
CXXFLAGS = -march=rv64gcv -O3 -Wall

# Archivos fuente
SRCS = comparation.cpp \
       ../Microbenchmarks/bindBenchmark.cpp \
       ../Microbenchmarks/hammingBenchmark.cpp \
       ../Microbenchmarks/queryBenchmark.cpp \
       ../DataGenerationScripts/randomUtils.cpp \
       ../BasicHDCKernels/scalarKernels.cpp \
       ../BasicHDCKernels/vectorialKernels.cpp \
       ../BasicHDCKernels/typeDefine.cpp

# Genera la lista de archivos objeto (.o) a partir de los .cpp
OBJS = $(SRCS:.cpp=.o)

# Nombre del ejecutable final
TARGET = comparation

# Regla principal
all: $(TARGET)

# Regla para enlazar el ejecutable a partir de los .o
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Regla para compilar cada .cpp individualmente a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar archivos generados
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
