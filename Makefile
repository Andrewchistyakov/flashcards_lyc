# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Source files and output
SOURCES = $(wildcard *.cpp)
OUTPUT = fcard

# Build target
$(OUTPUT): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(OUTPUT)

