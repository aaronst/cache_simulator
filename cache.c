#include "cache.h"

static cache_config config;
static cache c;

static struct {
	int valid = 0;
	int tag = 0;
	int data = 0;
} block;

struct {
	block sets[][];
} cache;

void initialize_cache(cache_config cc) {
	config = cc;

	/* Other initialization work here. */
	int numberOfAddresses = 1 << config.address_space;  	  // Obtain number of addresses
	int wordSize = 1 << (config.word_size * 4);				  // Obtain actual word size
	int numberOfWords = numberOfAddresses / wordSize; 		  // Obtain number of words
	int blockSize = 1 << config.block_size;					  // Obtain actual block size
	int numberOfBlocks = numberOfWords / blockSize;			  // Obtain number of blocks
	int numberOfSets = numberOfBlocks / config.associativity; // Obtain number of sets

	block suchBlocks[numberOfSets][config.associativity];	  // Initialize block array

	c = {suchBlocks}; // Initialize cache

	printf("A cache was initialized with %d sets (rows),\n%d blocks per set,\n%d words per block,\nand %d bits per word.",
			numberOfSets, config.associativity, blockSize, wordSize);
}

cache_result access_cache(unsigned int byte_address) {
	cache_result result;

	/* Do work here. */

	return result;
}

