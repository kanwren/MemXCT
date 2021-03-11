# ----- Make Macros -----

CXX = mpicxx
CXXFLAGS = -std=c++11 -fopenmp
OPTFLAGS = -O3 -march=native -ftree-vectorize -mavx2 -fopt-info-vec-all

NVCC = nvcc
NVCCFLAGS = -lineinfo -O3 -std=c++11 -ccbin=mpicxx -gencode arch=compute_70,code=sm_70 -Xcompiler -fopenmp

LD_FLAGS = -fopenmp -L/usr/local/cuda-11.1/lib64 -lcudart

TARGETS = memxct
OBJECTS = src/main.o src/raytrace.o src/kernels.o

# ----- Make Rules -----

all:	$(TARGETS)

%.o: %.cpp
	${CXX} ${CXXFLAGS} ${OPTFLAGS} $^ -c -o $@

%.o : %.cu
	${NVCC} ${NVCCFLAGS} $^ -c -o $@

memxct: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LD_FLAGS)

clean:
	rm -f $(TARGETS) src/*.o src/*.o.* *.txt *.bin core *.html *.xml
