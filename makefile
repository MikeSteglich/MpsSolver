
# Makefile for MpsSolver

export CXX := g++
#export CXX := clang

export CXXFLAGS := -std=c++17  -ggdb -w 

SUBDIRS := src/ToolsAndData #src/Solver src/Solution  



.PHONY: mpsSolver clean

all : mpsSolver

mpsSolver :
	$(MAKE) -C src

clean :
	$(MAKE) -C src clean



.PHONY: tgz

tgz :
	tar -czvf bak/cmpl2-`date +%Y%m%d`.tgz makefile mpsSolver.* \
		src/*.{cc,hh,d} src/makefile src/modules.reg bin/makefile  \
		$(SUBDIRS:%=%/*.{cc,hh,d}) $(SUBDIRS:%=%/makefile)  \
		#test/{makefile,test-all,test-cmpl} test/*.{cmpl,comp.mps,comp.stdout} \
		#doc/*.od?

test :
	$(MAKE) -C test test

