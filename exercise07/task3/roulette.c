#include "roulette.h"

// Define the roulette wheel with American roulette numbers and colors
void initializeWheel(RouletteNumber wheel[]) {
	// Color assignments according to standard American roulette
	int blacks[] = {2, 4, 6, 8, 10, 11, 13, 15, 17, 20, 22, 24, 26, 28, 29, 31, 33, 35};
	for (int i = 0; i < 38; i++) {
		if (i == 0 || i == 37) {  // 0 and 00
			wheel[i].number = i == 37 ? 0 : i;
			wheel[i].color = GREEN;
		} else {
			wheel[i].number = i;
			wheel[i].color = RED;  // Default to red
			for (long unsigned int j = 0; j < sizeof(blacks) / sizeof(blacks[0]); j++) {
				if (i == blacks[j]) {
					wheel[i].color = BLACK;
					break;
				}
			}
		}
	}
}

