EXE = unittest
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.opp, $(SRC))
DEP = $(patsubst %.cpp, %.dpp, $(SRC))

# C++ compiler flags
CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Werror
LDFLAGS =

# phony targets
.PHONY: all clean run

all: $(EXE)

clean:
	rm -rf $(EXE) $(OBJ) $(DEP)

run: all
	./$(EXE)


# main executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# .opp file
%.opp: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEP)

# .d file
%.dpp: %.cpp
	$(CC) $(CFLAGS) -MM $< > $@
