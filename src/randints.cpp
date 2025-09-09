#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

class Config {
    public:
        int seed;
        int max;
        int min;
        int count;
};

/* Usage message. */
void usage() {
    fprintf(stderr, 
            "usage: randints [--seed|-s] [--max|-x] [--min|-n] [--count|-c]\n");
    fprintf(stderr, "       Prints random integers.\n");
}

/* string -> int */
int string_to_int(const string &s) {
    int rv;

    if (sscanf(s.c_str(), "%d", &rv) != 1) {
        fprintf(stderr, "Bad integer: %s\n", s.c_str());
        exit(1);
    }

    return rv;
}

/* Parse command line flags. */
void parse_argv(int argc, char **argv, Config &cfg) {
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 ||
                strcmp(argv[i], "-h") == 0) {
            usage();
            exit(0);
        }

        if (strcmp(argv[i], "--seed") == 0 || 
                strcmp(argv[i], "-s") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Flag needs a value: %s\n", argv[i]);
                exit(1);
            }

            cfg.seed = string_to_int(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "--max") == 0 ||
                    strcmp(argv[i], "-x") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Flag needs a value: %s\n", argv[i]);
                exit(1);
            }

            cfg.max = string_to_int(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "--min") == 0 ||
                    strcmp(argv[i], "-n") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Flag needs a value: %s\n", argv[i]);
                exit(1);
            }

            cfg.min = string_to_int(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "--count") == 0 ||
                      strcmp(argv[i], "-c") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Flag needs a value: %s\n", argv[i]);
                exit(1);
            }

            cfg.count = string_to_int(argv[i + 1]);
            i++;
        }
        else {
            fprintf(stderr, "Bad flag: %s\n", argv[i]);
            exit(1);
        }
    }

    if (cfg.min > cfg.max) {
        fprintf(stderr, "Minimum value (%d) should be smaller than the maximum value (%d)\n", cfg.min, cfg.max);
        exit(1);
    }
    if (cfg.count < 0) {
        fprintf(stderr, "Count (%d) should be positive\n", cfg.count);
        exit(1);
    }
}

int main(int argc, char **argv) {
    Config cfg;
    int r;
    int i;

    cfg.seed = time(NULL);
    cfg.max = 0x7fffffff;
    cfg.min = 0x0;
    cfg.count = 1;

    parse_argv(argc, argv, cfg);

    srand48(cfg.seed);

    for (i = 0; i < cfg.count; i++) {
        r = lrand48();
        r %= (cfg.max - cfg.min + 1);
        r += cfg.min;
        printf("%d\n", r);
    }
}
