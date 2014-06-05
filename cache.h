/*
 * Aaron Stephens & Ben Foster
 * Homework 5
 * TCSS 372
 * 6/4/2014
 */

#ifndef _CACHE_H_
#define _CACHE_H_

typedef struct {
	unsigned int address_space;  /* in bits, between 1 and 32 (inclusive) */
	unsigned int associativity;	 /* 1, 2, or 4 */
	unsigned int word_size;      /* in bytes, a power of 2 */
	unsigned int block_size;     /* in words, a power of 2 */
	unsigned int cache_size;     /* in blocks, a power of 2 */
} cache_config;

typedef struct {
	unsigned int block_address;
	unsigned int index;          /* a.k.a. set number for an associative cache */
	unsigned int tag;
	char hit;                    /* 0 means false, not 0 means true */
} cache_result;

void initialize_cache(cache_config cc);
cache_result access_cache(unsigned int byte_address);

#endif
