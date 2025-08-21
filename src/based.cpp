#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

/* Notes for future-self:
 * - base 2 support would be nice, maybe even base 8??
 * - a "verbose" option that prints the settings every time they change
 */

class Config {
    public:
        char base; 
        int word_size;
        bool sign;
};

/* Checks for "--" at the start of a string. */
bool starts_with_dashes(const string &s) {
    if (s.size() <= 2) return false;
    return (s[0] == '-' && s[1] == '-');
}

/* Parses command flags, and changes settings accordingly. True if successful. */
bool parse_command_flag(const string &flag, Config &cfg) {
    string word_size_str;
    int word_size;

    /* Bases. */
    if (flag == "--dec") {
        cfg.base = 'D';
        return true;
    }
    else if (flag == "--hex") {
        cfg.base = 'H';
        return true;
    }

    /* Signedness. */
    if (flag.size() < 4) return false;
    
    if (flag[2] == 'i') {
        cfg.sign = true;
    }
    else if (flag[2] == 'u') {
        cfg.sign = false;
    }
    else return false;

    /* Size. */
    word_size_str = flag.substr(3);
    if (sscanf(word_size_str.c_str(), "%d", &word_size) != 1) return false;
    
    if (word_size != 8 && word_size != 16 && word_size != 32 && word_size != 64) return false;
    cfg.word_size = word_size;

    return true;
}

/* Prints the conversion. */
void print_conversion(long n, const Config &cfg) {
    string dec_side, hex_side, tmp;
    char buf[1024];
    unsigned long str_len, tmp_len;
    unsigned long mask, max, cur, signed_bit;

    max = 1UL << (unsigned long) cfg.word_size;
    signed_bit = 1UL << (unsigned long) (cfg.word_size - 1UL);
    mask = max - 1UL;

    //printf("DEBUG: m:%lu ms:%lu mk:%lu sb: %lx n:%lu ws:%d\n", max, max_signed, mask, signed_bit, n, cfg.word_size);
    
    /* Decimal. */
    if (cfg.word_size == 64) {
        if (cfg.sign) snprintf(buf, 1024, "%ld", n);
        else snprintf(buf, 1024, "%lu", n);
    }
    else if (cfg.sign) { /* Sign extend. */

        if (n & signed_bit) {
            cur = (max - n) * -1;
        }
        else {
            cur = n;
        }
        snprintf(buf, 1024, "%ld", cur);
    }
    else {
        cur = n & mask;
        snprintf(buf, 1024, "%lu", cur);
    }
    dec_side = buf;

    /* Hex. */
    str_len = cfg.word_size / 8 * 2;
    snprintf(buf, 1024, "%lx", n);
    tmp = buf;
    tmp_len = tmp.size();
    hex_side = "0x";
    if (tmp_len > str_len) hex_side += tmp.substr(tmp_len - (unsigned long) str_len);
    else hex_side += tmp;

    if (cfg.base == 'H') printf("%7s = %-7s\n", dec_side.c_str(), hex_side.c_str());
    else if (cfg.base == 'D') printf("%7s = %-7s\n", hex_side.c_str(), dec_side.c_str());
    else {
        fprintf(stderr, "print_conversion() failed\n");
        exit(1);
    }
}

void usage(const string &err) {
    fprintf(stderr, "usage: based [flags] <numbers> ...\n");
    if (err == "") {
        fprintf(stderr, 
                "       Convert between numerical bases. Call with --help for a list of flags.\n");
    }
    else if (err == "help") {
        fprintf(stderr, "Valid flags:\n");
        fprintf(stderr, "  Numerical bases:\n");
        fprintf(stderr, "  --dec         [Decimal]\n");
        fprintf(stderr, "  --hex         [Hexadecimal]\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "  Integer sizes:\n");
        fprintf(stderr, "  --i8 / --u8   [Signed / Unsigned Char]\n");
        fprintf(stderr, "  --i16 / --u16 [Signed / Unsigned Short]\n");
        fprintf(stderr, "  --i32 / --u32 [Signed / Unsigned Int]\n");
        fprintf(stderr, "  --i64 / --u64 [Signed / Unsigned Long]\n");
    }
    else fprintf(stderr, "%s\n", err.c_str());
}

int main(int argc, char **argv) {
    Config cfg;
    int i;
    bool parsed;
    unsigned long input, mask;
    string output_format, decimal_side;

    /* Pre-flight checks. */
    if (argc == 1) {
        usage("");
        exit(1);
    }
    else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-?") == 0) {
        usage("help");
        exit(1);
    }

    cfg.base = 'D';
    cfg.word_size = 32;
    cfg.sign = true;

    /* The main loop. */
    for (i = 1; i < argc; i++) {
        /* Is it a flag? */
        if (starts_with_dashes(argv[i])) {
            parsed = parse_command_flag(argv[i], cfg);

            if (!parsed) {
                fprintf(stderr, "Couldn't parse flag \"%s\"\n", argv[i]);
            }

            continue;
        }

        /* If not, we assume it's a number. */
        if (cfg.base == 'H') { /* Opposite of what's being input, ideally. */
            if (sscanf(argv[i], "%ld", &input) != 1) {
                fprintf(stderr, "Couldn't parse number \"%s\"\n", argv[i]);
                continue;
            }
        }
        else if (cfg.base == 'D') {
            if (sscanf(argv[i], "%lx", &input) != 1) {
                fprintf(stderr, "Couldn't parse number \"%s\"\n", argv[i]);
                continue;
            }
        }
        else {
            fprintf(stderr, "main loop failed: bad base\n");
            exit(1);
        }

        /* Now that we have it, we print it. */
        mask = (1UL << cfg.word_size) - 1UL;
        if (cfg.word_size != 64) print_conversion(input & mask, cfg);  /* Band-aid fix because the mask doesn't work on longs. */
        else print_conversion(input, cfg);
    }
}
