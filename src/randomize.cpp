#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <ctime>
using namespace std;

/* TODO: Might be worth adding options to use a set seed / take input from a 
 * file, but I've never needed that, so idk. */

int main(int argc, char **argv) {
    string line;
    multimap <double, string> order;
    multimap <double, string>::const_iterator oit;

    (void) argv; /* Supresses compiler warnings. */

    if (argc != 1) {
        fprintf(stderr, "usage: randomize -- input on stdin\n");
        fprintf(stderr, "       Randomizes the order of the input lines.\n");
        exit(1);
    }

    srand48(time(NULL));

    /* This is a really funny trick I learned from an old professor of mine. */

    while (getline(cin, line)) {
        /* Add lines into a treemap keyed on random numbers. */
        order.insert( make_pair(drand48(), line) );
    }

    /* And then use the order of the random numbers to decide the output. */
    for (oit = order.begin(); oit != order.end(); oit++) {
        printf("%s\n", oit->second.c_str());
    }
}
