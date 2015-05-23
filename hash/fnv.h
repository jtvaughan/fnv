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

#ifndef __CHIKARA_FNV_H
#define __CHIKARA_FNV_H

/* This file provides the Fowler-Noll-Vo (FNV) hash algorithm as described
   in <http://www.isthe.com/chongo/tech/comp/fnv/>. */

#include <stddef.h>
#include <stdint.h>

/* This is the recommended starting value ("offset basis") for 32-bit
   FNV hashes. */
#define CKR_FNV32_INIT  (2166136261)

/* This is the recommended starting value ("offset basis") for 64-bit
   FNV hashes. */
#define CKR_FNV64_INIT  (14695981039346656037ULL)

/* Compute the 32-bit FNV-1 hash of the specified data. */
extern uint32_t ckrFnv32(const void *data, size_t len);

/* Compute the 64-bit FNV-1 hash of the specified data. */
extern uint64_t ckrFnv64(const void *data, size_t len);

/* Compute the 32-bit FNV-1a hash of the specified data. */
extern uint32_t ckrFnv1a32(const void *data, size_t len);

/* Compute the 64-bit FNV-1a hash of the specified data. */
extern uint64_t ckrFnv1a64(const void *data, size_t len);

/* Compute the 32-bit FNV-1 hash of the specified data using *hash as
   the hash's initial value (its "offset basis") and the destination of
   the final hash value.  Use this instead of ckrFnv32 when you need to hash
   data in chunks rather than all at once (e.g., when you're streaming
   data).  The FNV designers recommend initializing *hash
   with CKR_FNV32_INIT. */
extern void ckrFnv32Chunked(const void *data, size_t len, uint32_t *hash);

/* Compute the 64-bit FNV-1 hash of the specified data using *hash as
   the hash's initial value (its "offset basis") and the destination of
   the final hash value.  Use this instead of ckrFnv64 when you need to hash
   data in chunks rather than all at once (e.g., when you're streaming
   data).  The FNV designers recommend initializing *hash
   with CKR_FNV64_INIT. */
extern void ckrFnv64Chunked(const void *data, size_t len, uint64_t *hash);

/* Compute the 32-bit FNV-1a hash of the specified data using *hash as
   the hash's initial value (its "offset basis") and the destination of
   the final hash value.  Use this instead of ckrFnv1a32 when you need to hash
   data in chunks rather than all at once (e.g., when you're streaming
   data).  The FNV designers recommend initializing *hash
   with CKR_FNV32_INIT. */
extern void ckrFnv1a32Chunked(const void *data, size_t len, uint32_t *hash);

/* Compute the 64-bit FNV-1a hash of the specified data using *hash as
   the hash's initial value (its "offset basis") and the destination of
   the final hash value.  Use this instead of ckrFnv1a64 when you need to hash
   data in chunks rather than all at once (e.g., when you're streaming
   data).  The FNV designers recommend initializing *hash
   with CKR_FNV64_INIT. */
extern void ckrFnv1a64Chunked(const void *data, size_t len, uint64_t *hash);

/* Compute the FNV-1 hash of the specified data using *hashptr as the
   hash's initial value (its "offset basis") and the destination of the
   final hash value.  This is a wrapper around the ckrFnv*Chunked functions.
   hashptr can point to any 32- or 64-bit integer, signed or unsigned. */
#define ckrFnv(data, len, hashptr) \
    _Generic((hashptr), \
        int32_t:  fnv32_chunked, \
        uint32_t: fnv32_chunked, \
        int64_t:  fnv64_chunked, \
        uint64_t: fnv64_chunked \
    )(data, len, _Generic((hashptr), \
                     int32_t:(uint32_t *)(hashptr), \
                     int64_t:(uint64_t *)(hashptr), \
                     default:(hashptr) \
                 ))

/* Compute the FNV-1a hash of the specified data using *hashptr as the
   hash's initial value (its "offset basis") and the destination of the
   final hash value.  This is a wrapper around the ckrFnv1a*Chunked functions.
   hashptr can point to any 32- or 64-bit integer, signed or unsigned. */
#define ckrFnv1a(data, len, hashptr) \
    _Generic((hashptr), \
        int32_t:  fnv1a32_chunked, \
        uint32_t: fnv1a32_chunked, \
        int64_t:  fnv1a64_chunked, \
        uint64_t: fnv1a64_chunked \
    )(data, len, _Generic((hashptr), \
                     int32_t:(uint32_t *)(hashptr), \
                     int64_t:(uint64_t *)(hashptr), \
                     default:(hashptr) \
                 ))

#endif  /* __CHIKARA_FNV_H */
