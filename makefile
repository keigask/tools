FLAGS = -Wall -Wextra

# Make commands.

all: bin/based bin/charm

clean:
	rm -f bin/* ; touch bin/.keep

# C++ programs.

bin/based: src/based.cpp
	g++ $(FLAGS) -o bin/based src/based.cpp

bin/charm: src/charm.cpp
	g++ $(FLAGS) -o bin/charm src/charm.cpp

# Shell scripts
# (none...)
