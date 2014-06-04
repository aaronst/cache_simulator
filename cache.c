#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cache.h"

typedef struct {
	int valid;
	int tag;
} block;

typedef struct {
	block **blocks;
} cache;

static cache_config config;
static int numberOfRows;
static cache c;

void initialize_cache(cache_config cc) {
	config = cc;

	/* Other initialization work here. */
	numberOfRows = config.cache_size / config.associativity;
	int i;

	block *set = (block*) calloc(config.associativity, sizeof(block));

	block **rows = (block**) calloc(numberOfRows, sizeof(set));

	for (i = 0; i < numberOfRows; i++) {
		rows[i] = (block*) calloc(config.associativity, sizeof(block));
	}

	c.blocks = rows;

	printf("Cache initialized with %d rows and %d blocks per row.\n", numberOfRows, config.associativity);
}

cache_result access_cache(unsigned int byte_address) {
	cache_result result;

	/* Do work here. */	
	unsigned int blockOffset = byte_address % config.associativity;

	unsigned int indexBits = (unsigned int) log(numberOfRows) / log(2) + 1;

	unsigned int index;
	unsigned int givenTag;
	if (config.associativity > 1) {
		index = (byte_address >> (config.associativity / 2)) % numberOfRows;
		givenTag = byte_address >> ((config.associativity / 2) + indexBits);
	} else {
		index = byte_address % numberOfRows;
		givenTag = byte_address >> indexBits;
	}

	block currentBlock = c.blocks[index][blockOffset];

	unsigned int currentTag = currentBlock.tag;

	char hit;
	if (givenTag == currentTag && currentBlock.valid == 1) {
		hit = 1;
	} else {
		hit = 0;
		c.blocks[index][blockOffset].tag = givenTag;
		c.blocks[index][blockOffset].valid = 1;
	}

	result.block_address = blockOffset;
	result.index = index;
	result.tag = currentTag;
	result.hit = hit;
	return result;
}

