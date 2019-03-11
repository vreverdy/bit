CXX = clang++
CXX_FLAGS = -Wall -std=c++17 
LD_FLAGS = 
OBJS = test_main.o test1.o
DEPS = catch2.hpp ../lib/bit.hpp 

%.o: %.cc $(DEPS) 
	$(CXX) $(CXX_FLAGS) -c $<

tests.out: $(OBJS)
	$(CXX) $(OBJS) -o $@

all: tests.out

test: tests.out
	./$<

clean: 
	rm *.o *.out
