CC := gcc
CXX := g++
#include /usr/share/make-common/common.1.mk

CXXFLAGS := -I./include -g
CFLAGS := --std=gnu99 -O0

#default: $(LIBFILES) $(BINFILES)

#$(call LINKBIN, fastrakd, fastrak.o fastrakd.o, amino somatic stdc++ reflex)

#.PHONY: default clean

BINARIES := liberty #fastrakd fastrak-test libertyd

all : $(BINARIES)

LIBS := -lach -lrt -lc -lm -lusb-1.0

PiTracker.o: src/PiTracker.cpp
	$(CXX) $(CXXFLAGS) -o $@ src/PiTracker.cpp $(LIBS)

LIBERTY_OBJS := src/liberty.o src/PiTracker.o

liberty: $(LIBERTY_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(LIBERTY_OBJS) $(LIBS) 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -fr $(BINARIES) src/*.o
depclean: clean
	rm -fr .deps
