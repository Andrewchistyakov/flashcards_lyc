# compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra

# source files and output
SOURCES = $(wildcard *.cpp)
OUTPUT = fcard

# build target
$(OUTPUT): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(OUTPUT)

# test program
test:
	python3 -m unittest discover -s tests

