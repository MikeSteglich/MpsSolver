
CXX ?= g++
CXXFLAGS ?=  -ggdb -std=c++0x -Wall -Wno-parentheses

MODOBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))

%.d: %.cc
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


mpsSolver : $(MODOBJS)

all : mpsSolver

clean:
	-rm *.o

include $(patsubst %.cc,%.d,$(wildcard *.cc))

