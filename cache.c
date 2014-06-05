#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cache.h"

typedef struct {
	int valid;
	int tag;
	int age;
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
	int i;

	unsigned int wordSize = config.word_size;
	unsigned int byteBits = 0;
	while (wordSize >>= 1) ++byteBits;

	unsigned int blockSize = config.block_size;
	unsigned int blockBits = 0;
	while (blockSize >>= 1) ++blockBits;

	unsigned int cacheSize = numberOfRows;
	unsigned int indexBits = 0;
	while (cacheSize >>= 1) ++indexBits;

	//unsigned int byteOffset = byte_address % byteBits;
	long blockOffset = (byte_address >> byteBits) % config.associativity;

	printf("Byte Bits:   %d\n", byteBits);
	printf("Block bits:  %d\n", blockBits);
	printf("Index Bits:  %d\n", indexBits);

	unsigned int index = (byte_address >> (byteBits + blockBits)) % numberOfRows;
	unsigned int givenTag = byte_address >> (byteBits + blockBits + indexBits);

	block currentBlock;

	char hit = 0;

	for (i = 0; i < config.associativity; i++) {
		currentBlock = c.blocks[index][i];
		if (givenTag == currentBlock.tag && currentBlock.valid == 1) {
			hit = 1;
			c.blocks[index][i].age = 0;
			break;
		}
	}

	block oldestBlock = c.blocks[index][0];
	int oldestBlockIndex = 0;
	int loaded = 0;

	if (hit == 0) {
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

	for (i = 0; i < config.associativity; i++) {
		if (c.blocks[index][i].valid == 1) {
			c.blocks[index][i].age++;
		}
	}

	result.block_address = (index * config.associativity) + blockOffset;
	result.index = index;
	result.tag = givenTag;
	result.hit = hit;
	return result;
}

