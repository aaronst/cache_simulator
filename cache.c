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

	double byteBits = log(config.word_size) / log(2);
	int byteB = byteBits;

	double blockBits = log(config.block_size) / log(2);
	int blockB;

	double indexBits = log(numberOfRows) / log(2);
	int iB = indexBits;

	unsigned int byteOffset = byte_address % byteB;
	unsigned int blockOffset = (byte_address >> byteB) % config.associativity;

	printf("Byte Bits:   %d\n", byteB);
	printf("Block bits:  %d\n", blockB);
	printf("Index Bits:  %d\n", iB);

	unsigned int index = (byte_address >> (byteB + blockB)) % numberOfRows;
	unsigned int givenTag = byte_address >> (byteB + blockB + iB);

	block currentBlock;

	char hit = 0;

	for (i = 0; i < config.associativity; i++) {
		currentBlock = c.blocks[index][i];
		if (givenTag == currentBlock.tag && currentBlock.valid == 1) {
			hit = 1;
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

	result.block_address = byte_address / (config.block_size * config.word_size);
	result.index = index;
	result.tag = givenTag;
	result.hit = hit;
	return result;
}

