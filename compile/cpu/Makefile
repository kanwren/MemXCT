# ----- Make Macros -----

CXX = mpicxx
CXXFLAGS = -std=c++11 -fopenmp
OPTFLAGS = -O3 -march=native -ftree-vectorize -mavx2 -fopt-info-vec-all

LD_FLAGS = -fopenmp

TARGETS = memxct
OBJECTS = src/main.o src/raytrace.o src/kernels.o

# ----- Make Rules -----

all:	$(TARGETS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c $^ -o $@

memxct: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LD_FLAGS)

clean:
	rm -f $(TARGETS) src/*.o src/*.o.* *.txt *.bin core
