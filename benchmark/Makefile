CXX = g++
CXX_FLAGS = -std=c++14 -Wall -Wextra -pedantic -g -O3 -mpopcnt -I$$PWD/../lib/
LD_FLAGS =
LIB = ../lib
DEPS = $(LIB)/bit.hpp $(LIB)/bit_algorithm.hpp $(LIB)/bit_details.hpp \
       $(LIB)/bit_iterator.hpp $(LIB)/bit_pointer.hpp $(LIB)/bit_reference.hpp \
       $(LIB)/bit_value.hpp $(LIB)/linear_overload.hpp

TARGS = bit.out

%.o: %.cc
	$(CXX) $(CXX_FLAGS) -c $<

%.out: %.o
	$(CXX) $< -o $@

all: $(TARGS)

clean:
	rm *.o *.out

