# makefile
# date 2017-09-13
# Copyright 2017 Mamoru kaminaga
TARGET = satpass.out
SRC = export.cc file.cc input.cc main.cc output.cc solution.cc
OBJDIR = build
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRC:.cc=.o)))

GCC = g++
INCLUDE = -I ~/projects/mkaminaga/ -I ~/projects/library/
CFLAGS = -std=c++0x -g -Wall -O2 -DUNICODE
LIBS = -lrt -lstdc++ ~/projects/library/tle/v2.1.0/libtle.a ~/projects/library/sat/v1.0.2/libsat.a
LDFLAGS =

$(TARGET): $(OBJS)
	$(GCC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

$(OBJDIR)/%.o: %.cc
	mkdir -p $(OBJDIR)
	$(GCC) $(CFLAGS) $(INCLUDE) -o $@ -c $^
