#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

static struct {
	cache_config config;
	int addresses[9];
} simulations[] = {{{5, 1, 1, 1, 8}, {22, 26, 22, 26, 16, 3, 16, 18, 16}}, /* Figures 5.8 & 5.9 */
                   {{32, 1, 4, 1, 1024}, {0, 1024, 2048, 5120, 1024, 98304, 0, 2048, 1024}}, /* Figure 5.10 */
                   {{32, 1, 4, 16, 256}, {24, 16, 8, 6400, 6464, 0, 16400, 50, 16388}}, /* Figure 5.12 */
                   {{10, 2, 1, 1, 8}, {8, 4, 8, 12, 4, 513, 0, 515, 12}}, /* Figure 5.14, with omitted cache parameters filled in. */
                   {{32, 4, 4, 1, 1024}, {0, 1024, 2048, 5120, 1024, 98304, 0, 2048, 1024}}}; /* Figure 5.18 */

#define SIMULATION_TO_RUN 4

int main(int argc, char* argv[]) {
	cache_result result;
	int i;

	/* Disable buffering of the standard output stream, removing the console
	   output delay in Eclipse that occurs on some systems. */
	setvbuf(stdout, NULL, _IONBF, 0);

	printf("Running simulation for the following cache configuration...\n");
	printf(" Address bits: %u\n", simulations[SIMULATION_TO_RUN].config.address_space);
	printf("Associativity: %u\n", simulations[SIMULATION_TO_RUN].config.associativity);
	printf("    Word size: %u byte(s)\n", simulations[SIMULATION_TO_RUN].config.word_size);
	printf("   Block size: %u word(s)\n", simulations[SIMULATION_TO_RUN].config.block_size);
	printf("   Cache size: %u block(s)\n\n", simulations[SIMULATION_TO_RUN].config.cache_size);

	initialize_cache(simulations[SIMULATION_TO_RUN].config);

	for (i = 0; i < 9; i++) {
		result = access_cache(simulations[SIMULATION_TO_RUN].addresses[i]);
		printf("Byte addr: %u  -->  Block addr: %u  Index: %u  Tag: %u  Hit: %d\n",
				simulations[SIMULATION_TO_RUN].addresses[i], result.block_address,
				result.index, result.tag, (int)result.hit);
	}

	return EXIT_SUCCESS;
}
