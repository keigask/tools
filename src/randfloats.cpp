#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

class Config {
    public:
        int seed;
        double max;
        double min;
        int count;
};

/* Usage message. */
void usage() {
    fprintf(stderr, 
            "usage: randfloats [--seed|-s] [--max|-x] [--min|-n] [--count|-c]\n");
    fprintf(stderr, "       Prints random floats.\n");
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

/* string -> double */
double string_to_double(const string &s) {
    double rv;

    if (sscanf(s.c_str(), "%lf", &rv) != 1) {
        fprintf(stderr, "Bad float: %s\n", s.c_str());
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

        if (i + 1 >= argc) {
            fprintf(stderr, "Flag needs a value: %s\n", argv[i]);
        }

        if (strcmp(argv[i], "--seed") == 0 || 
                strcmp(argv[i], "-s") == 0) {
            cfg.seed = string_to_int(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "--max") == 0 ||
               strcmp(argv[i], "-x") == 0) {
            cfg.max = string_to_double(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "--min") == 0 ||
               strcmp(argv[i], "-n") == 0) {
            cfg.min = string_to_double(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "--count") == 0 ||
                 strcmp(argv[i], "-c") == 0) {
            cfg.count = string_to_int(argv[i + 1]);
            i++;
        }
        else {
            fprintf(stderr, "Bad flag: %s\n", argv[i]);
            exit(1);
        }
    }
}

int main(int argc, char **argv) {
    Config cfg;
    double r;
    int i;

    cfg.seed = time(NULL);
    cfg.max = 1.0;
    cfg.min = 0.0;
    cfg.count = 1;

    parse_argv(argc, argv, cfg);

    srand48(cfg.seed);

    for (i = 0; i < cfg.count; i++) {
        r = drand48();
        r *= (cfg.max - cfg.min);
        r += cfg.min;
        printf("%lf\n", r);
    }
}
