

export CXX ?= g++
export CXXFLAGS ?=  -std=c++17  -ggdb -w 


MPSSOLVERMAKE := makefile

OUTPUT = ../bin/MpsSolver

# INCLUDES := $(wildcard Control/*.hh)

OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))
LIBS :=

SUBDIRS := ToolsAndData Solver Solution 
#SUBDIRS := ToolsAndData


%.d: %.cc
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


$(OUTPUT) : $(OBJS) subobjs
	$(CXX) -o $(OUTPUT) $(OBJS) $(SUBDIRS:%=%/*.o) $(LIBS) 

all : $(OUTPUT)



.PHONY: subobjs $(SUBDIRS)
 
subobjs : $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@


include $(patsubst %.cc,%.d,$(wildcard *.cc))



.PHONY: clean

clean :
	-rm *.o
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done


.PHONY: tgz

tgz :
	$(MAKE) -C .. tgz

test :
	$(MAKE) -C ../test test

