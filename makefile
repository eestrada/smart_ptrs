BIN = ./test.bin

INCDIR = ./inc
OBJDIR = ./obj
SRCDIR = ./src

INCS = $(INCDIR)/memory.hpp
OBJS = $(OBJDIR)/test.o

INCLUDES = -Iinc/
CXX ?= c++
CXXFLAGS = -std=c++0x -O3 -g -Wall -pedantic $(INCLUDES)

.PHONY : run bin test clean memcheck

run : $(BIN)
	@ echo "Testing executable"
	$(BIN)

bin : $(BIN)

test : clean memcheck

clean :
	@ echo "Removing generated files"
	@rm -vf $(BIN)
	@rm -vrf $(OBJDIR)

memcheck : $(BIN) 
	@ echo "Running valgrind to check for memory leaks"
	valgrind --tool=memcheck --leak-check=yes --max-stackframe=5000000 \
	--show-reachable=yes $(BIN)
	@ echo

$(BIN) : $(OBJS) $(INCS)
	@ echo "Compiling binary"
	$(CXX) -o $(BIN) $(OBJS) $(CXXFLAGS)
	@ echo

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(INCS)
	@- mkdir -p $(OBJDIR)
	@ echo "Compiling object file: " $@
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@ echo

