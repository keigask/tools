#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unordered_set>
#include <cstring>
#include <ctime>
#include "names.hpp"
using namespace std;

class Config {
    public:
        int count;
        int seed;
        char gender;
};

/* Random number in the range [0, max) */
int rand_int(int max) {
    return lrand48() % max;
}

/* Generate a name. */
string gen_name(char gender) {
    string name;
    char true_gender;

    name = "";

    if (gender != 'x') true_gender = gender;
    else true_gender = (rand_int(2) == 0) ? 'm' : 'f';

    /* First name. */
    if (true_gender == 'm') name += male_names[lrand48() % male_names.size()];
    else name += female_names[lrand48() % female_names.size()];

    /* Middle names. */
    if (rand_int(5) == 0) {
        name += " ";
        if (true_gender == 'm') name += male_names[lrand48() % male_names.size()];
        else name += female_names[lrand48() % female_names.size()];
    }

    /* Last name. */
    name += " ";

    if (rand_int(10) == 0) name += last_prefixes[lrand48() % last_prefixes.size()];
    name += last_names[lrand48() % last_names.size()];
    if (rand_int(15) == 0) {
        name += "-";
        if (rand_int(10) == 0) name += last_prefixes[lrand48() % last_prefixes.size()];
        name += last_names[lrand48() % last_names.size()];
    }

    if (rand_int(20) == 0) {
        name += " ";
        if (true_gender == 'm') name += male_suffixes[lrand48() % male_suffixes.size()];
        else name += female_suffixes[lrand48() % female_suffixes.size()];
    }

    return name;
}

/* Edit config with flag values. */
bool parse_flag(const string &flag, const string &value, Config &cfg) {
    int n;

    if (flag == "--count" || flag == "-n") {
        if (sscanf(value.c_str(), "%d", &n) != 1) return false;
        cfg.count = n;
        return true;
    }
    else if (flag == "--seed" || flag == "-s") {
        if (sscanf(value.c_str(), "%d", &n) != 1) return false;
        cfg.seed = n;
        return true;
    }
    else if (flag == "--gender" || flag == "-g") {
        if (value != "m" && value != "f" && value != "x") return false;
        if (value.size() == 0) return false;
        cfg.gender = value[0];
        return true;
    }

    return false;
}

/* Usage text. */
void usage() {
    fprintf(stderr, "usage: randnames [flags ...]\n");
    fprintf(stderr, "       Generates random names.\n");
    fprintf(stderr, "Legal flags:\n");
    fprintf(stderr, "  --count #  -n # -- number of names to output (default: 1)\n");
    fprintf(stderr, "  --seed #   -s # -- seed for rng (default: system time)\n");
    fprintf(stderr, "  --gender # -g # -- gender for names m|f|x (default: x [any])\n");
    fprintf(stderr, "  --help     -h   -- print this message and exit\n");
}

int main(int argc, char **argv) {
    Config cfg;
    int i;
    string flag, val, name;
    bool parsed;
    unordered_set<string> used;

    /* Parse command line arguments. */

    cfg.count = 1;
    cfg.seed = -1;
    cfg.gender = 'x';

    for (i = 1; i < argc; i++) {
        flag = argv[i];

        if (flag == "--help" || flag == "-h") {
            usage();
            exit(1);
        }

        /* TODO: This runs on invalid flags. Honestly, I should just rewrite 
         * all of these to handle command line args the way randfloats does... 
         * */
        if ((i + 1) >= argc) {
            fprintf(stderr, "Error: flag %s requires a value\n", flag.c_str());
            exit(1);
        }
        else val = argv[i + 1];

        parsed = parse_flag(flag, val, cfg);
        if (!parsed) {
            fprintf(stderr, "Error: bad flag: %s %s\n", flag.c_str(), val.c_str());
            exit(1);
        }

        i++;
    }

    /* Generate names. */

    if (cfg.seed == -1) srand48(time(NULL));
    else srand48(cfg.seed);

    for (i = 0; i < cfg.count; i++) {
        do { 
            name = gen_name(cfg.gender);
        }
        while (used.find(name) != used.end());
        cout << name << endl;
        used.insert(name);
    }
}
