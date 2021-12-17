SRCDIR = ./src
INCDIR = ./include
LIBDIR = ./lib
BINDIR = ./bin
OBJDIR = ./obj
MAIN = ./main.cpp


LIB = $(LIBDIR)/libaoc.a
EXE = $(BINDIR)/aoc.exe
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -O3 -Winline
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

$(EXE): $(LIB) $(MAIN) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(MAIN) $(LDFLAGS)

clean:
	-del /q obj\*
	-del /q bin\*
	-del /q lib\*

all: $(EXE)

.PHONY: all