FLAGS = -Wall -Wextra -Iinclude

# Make commands.

all: bin/based bin/charm

clean:
	rm -f bin/* ; touch bin/.keep

# I keep this one out of 'make all' because some compilers don't like it...
randnames: bin/randnames

# C++ programs.

bin/based: src/based.cpp
	g++ $(FLAGS) -o bin/based src/based.cpp

bin/charm: src/charm.cpp
	g++ $(FLAGS) -o bin/charm src/charm.cpp

bin/randnames: src/randnames.cpp
	g++ $(FLAGS) -o bin/randnames src/randnames.cpp

# Shell scripts
# (none...)
