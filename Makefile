CXXFLAGS := -Wall -g

all: test

.PHONY: clean
clean:
	rm -f *.o test

test: MurmurHash3.o MurmurInc.o test.o
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@
