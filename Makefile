CC := gcc
CXX := g++
#include /usr/share/make-common/common.1.mk

CXXFLAGS := -I./include -g -Wall
CFLAGS := --std=gnu99 -O0 -I./include -g -Wall

#default: $(LIBFILES) $(BINFILES)

#$(call LINKBIN, fastrakd, fastrak.o fastrakd.o, amino somatic stdc++ reflex)

#.PHONY: default clean

BINARIES := libertyd #liberty-test

all : $(BINARIES)

LIBS := -lach -lrt -lc -lm -lusb-1.0
#LIBS := -lrt -lc -lm -lusb-1.0

PiTracker.o: src/PiTracker.cpp
	$(CXX) $(CXXFLAGS) -o $@ src/PiTracker.cpp $(LIBS)

LIBERTY_OBJS := src/daemonizer.o src/liberty.o src/PiTracker.o src/libertyd.o

libertyd: $(LIBERTY_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(LIBERTY_OBJS) $(LIBS) 

#liberty-test: $(LIBERTY_OBJS)
#	$(CC) $(CFLAGS) -o $@ $(LIBERTY_OBJS) $(LIBS) 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

install: libertyd
	install -D $(BINARIES) /usr/bin/$(BINARIES)

.PHONY: install

clean:
	rm -fr $(BINARIES) src/*.o
depclean: clean
	rm -fr .deps
