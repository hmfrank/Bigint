EXE = unittest
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.opp, $(SRC))
DEP = $(patsubst %.cpp, %.dpp, $(SRC))
EXT = catch.hpp

# C++ compiler flags
CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Werror
LDFLAGS =

# phony targets
.PHONY: all clean destroy run

all: $(EXE)

clean:
	rm -rf $(EXE) $(OBJ) $(DEP)

destroy: clean
	rm -rf $(EXT)

run: all
	./$(EXE)


# main executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# .opp file
%.opp: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEP)

# .dpp file
%.dpp: %.cpp
	$(CC) $(CFLAGS) -MM $< -MT $(patsubst %.cpp, %.opp, $<) -MG -MF $@

# external sources
catch.hpp:
	wget -q "https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp" -O $@