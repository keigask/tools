#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iomanip>
using namespace std;

class Config {
    public:
        char base_mode; /* 'H' for hex, 'D' for dec. */
        char output_mode; /* 'A' for ascii, 'N' for number. */
        bool mod_input; /* true = mod input ints by 128 */
};

/* Read flags and adjust config. */
bool parse_flags(const string &flag, Config &cfg) {
    if (flag == "--hex" || flag == "-x") {
        cfg.base_mode = 'H';
        return true;
    }
    else if (flag == "--dec" || flag == "-d") {
        cfg.base_mode = 'D';
        return true;
    }
    else if (flag == "--to-ascii" || flag == "-a") {
        cfg.output_mode = 'A';
        return true;
    }
    else if (flag == "--from-ascii" || flag == "-n") {
        cfg.output_mode = 'N';
        return true;
    }
    else if (flag == "--mod-input" || flag == "-m") {
        cfg.mod_input = true;
        return true;
    }
    else if (flag == "--no-mod-input" || flag == "-nm") {
        cfg.mod_input = false;
        return true;
    }


    return false;
}

/* String to hex. */
int parse_hex(const string &input) {
    int rv;
    istringstream sin;

    sin.str(input);
    if (!(sin >> hex >> rv)) return -1;
    return rv;
}

/* String to dec. */
int parse_dec(const string &input) {
    int rv;
    istringstream sin;

    sin.str(input);
    if (!(sin >> rv)) return -1;
    return rv;
}

/* Number to char value. */
string index_ascii_table(int index) {
    string rv;
    char c;

    if (index < 0 || index > 127) return "Invalid index";

    /* Normal printable characters. */
    if (index >= 0x21 && index <= 0x7e) {
        c = index;
        rv = c;
        return rv;
    }

    /* Special characters. */
    switch (index) {
        case  0: return "NUL '\\0' (null character)";
        case  1: return "SOH (start of heading)";
        case  2: return "STX (start of text)";
        case  3: return "ETX (end of text)";
        case  4: return "EOT (end of transmission)";
        case  5: return "ENQ (enquiry)";
        case  6: return "ACK (acknowledge)";
        case  7: return "BEL '\\a' (bell)";
        case  8: return "BS  '\\b' (backspace)";
        case  9: return "HT  '\\t' (horizontal tab)";
        case 10: return "LF  '\\n' (new line)";
        case 11: return "VT  '\\v' (vertical tab)";
        case 12: return "FF  '\\f' (form feed)";
        case 13: return "CR  '\\r' (carriage ret)";
        case 14: return "SO  (shift out)";
        case 15: return "SI  (shift in)";
        case 16: return "DLE (data link escape)";
        case 17: return "DC1 (device control 1)";
        case 18: return "DC2 (device control 2)";
        case 19: return "DC3 (device control 3)";
        case 20: return "DC4 (device control 4)";
        case 21: return "NAK (negative ack.)";
        case 22: return "SYN (synchronous idle)";
        case 23: return "ETB (end of trans. blk)";
        case 24: return "CAN (cancel)";
        case 25: return "EM  (end of medium)";
        case 26: return "SUB (substitute)";
        case 27: return "ESC (escape)";
        case 28: return "FS  (file separator)";
        case 29: return "GS  (group separator)";
        case 30: return "RS  (record separator)";
        case 31: return "US  (unit separator)";
        case 32: return "' ' (space)";
        case 127: return "DEL";
    }

    return "Error";
}

/* Help text. */
void usage(bool help_flag) {
    fprintf(stderr, "usage: charm <flags> <input>\n");
    fprintf(stderr, "       Convert between ASCII values and numbers.\n");
    if (!help_flag) fprintf(stderr, "       Call with --help for a list of flags.\n");
    else {
        fprintf(stderr, "Flags:\n");
        fprintf(stderr, "  base modes:\n"); 
        fprintf(stderr, "  --hex -x           -- input / output numbers should be base 16\n");
        fprintf(stderr, "  --dec -d           -- input / output numbers should be base 10\n");
        fprintf(stderr, "\n"); 
        fprintf(stderr, "  output modes:\n"); 
        fprintf(stderr, "  --to-ascii -a      -- output ascii from numbers\n");
        fprintf(stderr, "  --from-ascii -n    -- output numbers from ascii string\n");
        fprintf(stderr, "\n"); 
        fprintf(stderr, "  other:\n"); 
        fprintf(stderr, "  --mod-input -m     -- mod input numbers by 128\n");
        fprintf(stderr, "  --no-mod-input -dm -- don't mod input numbers, flag an error instead\n");
    }
}

/* Blow it up. */
void fatal_error(const string &err) {
    fprintf(stderr, "%s\n", err.c_str());
    exit(1);
}

int main(int argc, char **argv) {
    Config cfg;
    int i, j, cur;
    unsigned long str_len;
    string left_part, right_part;
    char buf[256];

    /* Setup. */

    if (argc == 1) {
        usage(false);
        exit(1);
    }
    else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        usage(true);
        exit(1);
    }

    cfg.base_mode = 0;
    cfg.output_mode = 0;
    cfg.mod_input = false;

    /* The loop. */
    for (i = 1; i < argc; i++) {
        if (parse_flags(argv[i], cfg)) continue;
        
        if (cfg.output_mode == 'A') {
            /* Number to ASCII value. */
            if (cfg.base_mode == 'H') {
                cur = parse_hex(argv[i]);
                if (cfg.mod_input) cur %= 128;
                snprintf(buf, 256, "0x%x", cur);
                left_part = buf;
            }
            else if (cfg.base_mode == 'D') {
                cur = parse_dec(argv[i]);
                if (cfg.mod_input) cur %= 128;
                snprintf(buf, 256, "%d", cur);
                left_part = buf;
            }
            else fatal_error("Error: no base mode set.");

            if (cur == -1) {
                fprintf(stderr, "Couldn't parse input: %s\n", argv[i]);
                continue;
            }

            right_part = index_ascii_table(cur);

            printf("%7s = %-7s\n", left_part.c_str(), right_part.c_str());
        }
        else if (cfg.output_mode == 'N') {
            /* ASCII strings to numbers. */
            str_len = strlen(argv[i]);

            for (j = 0; (unsigned long) j < str_len; j++) {
                left_part = argv[i][j];
                if (cfg.base_mode == 'H') snprintf(buf, 256, "0x%x", argv[i][j]);
                else if (cfg.base_mode == 'D') snprintf(buf, 256, "%d", argv[i][j]);
                else {
                    fatal_error("Error: no base mode set.");
                }
                right_part = buf;
            
                printf("%7s = %-7s\n", left_part.c_str(), right_part.c_str());
            }
        }
        else {
            fatal_error("main loop failed: bad output_mode");
        }

    }
}
