/*
 * Compute Fowler-Noll-Vo (FNV) Hashes for Arguments and Standard Input
 * Written in 2015 by Jordan Vaughan
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hash/fnv.h"

static const char *format;

static void print_short_usage(FILE *stream) {
    (void) fputs("Usage: fnv [-a] [-h] [-u] [-X] [-1] [-32] [STRINGS]\n",
      stream);
}

static void print_help(FILE *stream) {
    (void) fputs("\n\
Description:\n\
\n\
    Hash each string in STRINGS with the Fowler-Noll-Vo (FNV) hash\n\
    algorithm and print the results, one hash per line per string.\n\
    If STRINGS does not exist, then hash each line from standard input\n\
    individually and print the results one per line, or hash all of\n\
    standard input and print one result if -a or -b is specified.\n\
\n\
Options:\n\
\n\
    -a                     hash all of standard input rather than each line\n\
                           (meaningless if STRINGS are specified)\n\
    -h                     show this long help message and exit\n\
    -u                     disable standard output buffering\n\
    -X                     print hashes with uppercase hex letters instead\n\
                           of lowercase ones\n\
    -1                     use the FNV-1 algorithm rather than FNV-1a\n\
    -32                    compute 32-bit hashes instead of 64-bit ones\n\
\n", stream);
}

static void hash32(const void *data, size_t len) {
    uint32_t hash;

    hash = ckrFnv32(data, len);
    if (fprintf(stdout, format, hash) < 0) {
        exit(EXIT_FAILURE);
    }
}

static void hash64(const void *data, size_t len) {
    uint64_t hash;

    hash = ckrFnv64(data, len);
    if (fprintf(stdout, format, hash) < 0) {
        exit(EXIT_FAILURE);
    }
}

static void hash1a32(const void *data, size_t len) {
    uint32_t hash;

    hash = ckrFnv1a32(data, len);
    if (fprintf(stdout, format, hash) < 0) {
        exit(EXIT_FAILURE);
    }
}

static void hash1a64(const void *data, size_t len) {
    uint64_t hash;

    hash = ckrFnv1a64(data, len);
    if (fprintf(stdout, format, hash) < 0) {
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    void (*hash)(const void *, size_t);
    void (*partial_hash32)(const void *, size_t, uint32_t *);
    void (*partial_hash64)(const void *, size_t, uint64_t *);
    int n;
    char errors;
    char print_32bit_hashes;
    char print_uppercase_hex;
    char use_fnv1;
    char hash_all_stdin;
    char *line;
    size_t linecap;
    ssize_t numread;

    hash = hash1a64;
    partial_hash32 = ckrFnv1a32Chunked;
    partial_hash64 = ckrFnv1a64Chunked;
    format = "%016llx\n";
    errors = print_32bit_hashes = print_uppercase_hex = use_fnv1 =
      hash_all_stdin = 0;
    for (n = 1; n < argc; ++n) {
        if (strcmp(argv[n], "-a") == 0) {
            hash_all_stdin = 1;
        } else if (strcmp(argv[n], "-h") == 0) {
            print_short_usage(stdout);
            print_help(stdout);
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[n], "-u") == 0) {
            if (setvbuf(stdout, NULL, _IONBF, 0) != 0) {
                (void) fprintf(stderr, "error: couldn't disable standard output buffering");
                errors = 1;
            }
        } else if (strcmp(argv[n], "-X") == 0) {
            format = (print_32bit_hashes ? "%08X\n" : "%016llX\n");
            print_uppercase_hex = 1;
        } else if (strcmp(argv[n], "-1") == 0) {
            hash = (print_32bit_hashes ? hash32 : hash64);
            partial_hash32 = ckrFnv32Chunked;
            partial_hash64 = ckrFnv64Chunked;
            use_fnv1 = 1;
        } else if (strcmp(argv[n], "-32") == 0) {
            format = (print_uppercase_hex ? "%08X\n" : "%08x\n");
            hash = (use_fnv1 ? hash32 : hash1a32);
            print_32bit_hashes = 1;
        } else {
            break;
        }
    }
    if (errors) {
        exit(EXIT_FAILURE);
    }

    line = NULL;
    linecap = 0;
    if (n != argc) {
        for (; n < argc; ++n) {
            hash(argv[n], strlen(argv[n]));
        }
        exit(EXIT_SUCCESS);
    } else if (hash_all_stdin) {
        if (print_32bit_hashes) {
            uint32_t hashval;

            hashval = CKR_FNV32_INIT;
            while ((numread = getline(&line, &linecap, stdin)) >= 0) {
                partial_hash32(line, (size_t)numread, &hashval);
            }
            if (fprintf(stdout, format, hashval) < 0) {
                exit(EXIT_FAILURE);
            }
        } else {
            uint64_t hashval;

            hashval = CKR_FNV64_INIT;
            while ((numread = getline(&line, &linecap, stdin)) >= 0) {
                partial_hash64(line, (size_t)numread, &hashval);
            }
            if (fprintf(stdout, format, hashval) < 0) {
                exit(EXIT_FAILURE);
            }
        }
    } else {
        while ((numread = getline(&line, &linecap, stdin)) >= 0) {
            if (numread > 0 && line[numread - 1] == '\n') {
                --numread;
            }
            hash(line, (size_t)numread);
        }
    }

    free(line);
    return 0;
}
