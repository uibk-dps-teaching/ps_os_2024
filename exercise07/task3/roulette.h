#ifndef ROULETTE_H
#define ROULETTE_H

typedef enum {
	RED,
	BLACK,
	GREEN
} Color;

// Roulette number structure
typedef struct {
	int number;
	Color color;
} RouletteNumber;

void initializeWheel(RouletteNumber wheel[]);

#endif
