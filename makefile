FLAGS = -Wall -Wextra

# Make commands.

all: bin/based

clean:
	rm -f bin/* ; touch bin/.keep

# C++ programs.

bin/based: src/based.cpp
	g++ $(FLAGS) -o bin/based src/based.cpp

# Shell scripts
# (none...)
