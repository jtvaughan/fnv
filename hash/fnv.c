/*
 * 力 C Library
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

#include "fnv.h"

uint32_t ckrFnv32(const void *data, size_t len) {
    uint32_t hash;
    hash = CKR_FNV32_INIT;
    ckrFnv32Chunked(data, len, &hash);
    return hash;
}

uint64_t ckrFnv64(const void *data, size_t len) {
    uint64_t hash;
    hash = CKR_FNV64_INIT;
    ckrFnv64Chunked(data, len, &hash);
    return hash;
}

uint32_t ckrFnv1a32(const void *data, size_t len) {
    uint32_t hash;
    hash = CKR_FNV32_INIT;
    ckrFnv1a32Chunked(data, len, &hash);
    return hash;
}

uint64_t ckrFnv1a64(const void *data, size_t len) {
    uint64_t hash;
    hash = CKR_FNV64_INIT;
    ckrFnv1a64Chunked(data, len, &hash);
    return hash;
}

void ckrFnv32Chunked(const void *data, size_t len, uint32_t *hash) {
    const void *end;

    uint32_t temphash = *hash;
    for (end = data + len; data < end; ++data) {
        temphash *= 16777619;
        temphash ^= *(const char *)data;
    }
    *hash = temphash;
}

void ckrFnv64Chunked(const void *data, size_t len, uint64_t *hash) {
    const void *end;

    uint64_t temphash = *hash;
    for (end = data + len; data < end; ++data) {
        temphash *= 1099511628211ULL;
        temphash ^= *(const char *)data;
    }
    *hash = temphash;
}

void ckrFnv1a32Chunked(const void *data, size_t len, uint32_t *hash) {
    const void *end;

    uint32_t temphash = *hash;
    for (end = data + len; data < end; ++data) {
        temphash ^= *(const char *)data;
        temphash *= 16777619;
    }
    *hash = temphash;
}

void ckrFnv1a64Chunked(const void *data, size_t len, uint64_t *hash) {
    const void *end;

    uint64_t temphash = *hash;
    for (end = data + len; data < end; ++data) {
        temphash ^= *(const char *)data;
        temphash *= 1099511628211ULL;
    }
    *hash = temphash;
}
