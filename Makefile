SRCDIR = ./src
INCDIR = ./include
LIBDIR = ./lib
BINDIR = ./bin
OBJDIR = ./obj
MAIN = ./main.cpp


LIB = $(LIBDIR)/libaoc.a
EXE = $(BINDIR)/aoc
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -O3
LDFLAGS = -Llib -laoc

$(OBJDIR):
	mkdir $@

$(LIBDIR):
	mkdir $@

$(BINDIR):
	mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.hpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(LIB): $(OBJS) | $(LIBDIR)
	ar crs $(LIB) $(OBJS)

$(EXE): $(LIB) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(MAIN) $(LDFLAGS)

clean:
	-rm -f obj/*
	-rm -f bin/*
	-rm -f lib/*

all: $(EXE)

.PHONY: all