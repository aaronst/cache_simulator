/*
 * Aaron Stephens & Ben Foster
 * Homework 5
 * TCSS 372
 * 6/4/2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cache.h"

// Structure that represents one block in the cache.
typedef struct {
	int valid;		// Determines whether this block contains a valid entry or not
	int tag;		// Contains the tag for this block
	int age;		// Determines the age of this block
} block;

// Strucure that represents the cache
typedef struct {
	block **blocks; // A 2D array that contains all the blocks within the cache
} cache;

static cache_config config;	// The configuration for this cache
static int numberOfRows;	// The number of rows in this cache
static cache c;				// The cache being used for this simulation

/*
 * This function creates the 2D block array and assigns it to 'c', the cache
 */
void initialize_cache(cache_config cc) {
	config = cc;

	/* Other initialization work here. */
	int i;

	numberOfRows = config.cache_size / config.associativity;

	block *set = (block*) calloc(config.associativity, sizeof(block));

	block **rows = (block**) calloc(numberOfRows, sizeof(set));

	for (i = 0; i < numberOfRows; i++) {
		rows[i] = (block*) calloc(config.associativity, sizeof(block));
	}

	c.blocks = rows;
}

/*
 * This function reads in an address and determines the block address, index,
 * and tag of the given address.  Then, it determines whether or not that
 * specific address would result in a hit or not.  If it does not result in
 * a hit, the function updates the cache accordingly by manipulating the 2D
 * array 'blocks' in 'c'.
 */
cache_result access_cache(unsigned int byte_address) {
	cache_result result;

	/* Do work here. */
	int i;

	unsigned int wordSize = config.word_size;	// What these lines are doing
	unsigned int byteBits = 0;					// is determining how many
	while (wordSize >>= 1) ++byteBits;			// bits need to be ignored
												// because of the byte offset
	unsigned int blockSize = config.block_size;	// and block offset partitions
	unsigned int blockBits = 0;					// of the given address, as
	while (blockSize >>= 1) ++blockBits;		// well as how many bits to
												// use for the index.  The
	unsigned int cacheSize = numberOfRows;		// while loops are a weird
	unsigned int indexBits = 0;					// substitute for logarithmic
	while (cacheSize >>= 1) ++indexBits;		// operations I found online.

	// This determines which block in a given row the address is asking for.
	long blockOffset = (byte_address >> byteBits) % config.associativity;

	unsigned int index = (byte_address >> (byteBits + blockBits)) % numberOfRows;
	unsigned int givenTag = byte_address >> (byteBits + blockBits + indexBits);

	block currentBlock;

	char hit = 0;

	/*
	 * This loop determines if the given address will result in a hit or not.
	 * If there is a hit, the age of the block is renewed back to 0.
	 */
	for (i = 0; i < config.associativity; i++) {
		currentBlock = c.blocks[index][i];
		if (givenTag == currentBlock.tag && currentBlock.valid == 1) {
			hit = 1;
			c.blocks[index][i].age = 0;
			break;
		}
	}

	/*
	 * This next bit of code is only for when the given address results in a
	 * miss.  It determines a) If there is an empty block that can be filled,
	 * fill it, then b) If not, find the least recently used block and update
	 * it. 
	 */
	if (hit == 0) {
		block oldestBlock = c.blocks[index][0];
		int oldestBlockIndex = 0;
		int loaded = 0;

		for (i = 0; i < config.associativity; i++) {
			if (c.blocks[index][i].valid == 0) {
				c.blocks[index][i].tag = givenTag;
				c.blocks[index][i].valid = 1;
				loaded = 1;
				break;
			} else {
				if (c.blocks[index][i].age > oldestBlock.age) {
					oldestBlock = c.blocks[index][i];
					oldestBlockIndex = i;
				}
			}
		}
		if (loaded == 0) {
			c.blocks[index][oldestBlockIndex].tag = givenTag;
			c.blocks[index][oldestBlockIndex].age = 0;
		}
	}

	// Increment the age of all the valid blocks at this index.
	for (i = 0; i < config.associativity; i++) {
		if (c.blocks[index][i].valid == 1) {
			c.blocks[index][i].age++;
		}
	}

	// Assign the appropriate return values and return the result.
	result.block_address = (index * config.associativity) + blockOffset;
	result.index = index;
	result.tag = givenTag;
	result.hit = hit;
	return result;
}

