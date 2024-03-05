#ifndef COLORS_H
#define COLORS_H

#define RED_FG "\033[1;31m"
#define YELLOW_FG "\033[1;33m"
#define GREEN_FG "\033[1;32m"
#define BLUE_FG "\033[1;34m"
#define MAGENTA_FG "\033[1;35m"
#define CYAN_FG "\033[1;36m"
#define WHITE_FG "\033[1;37m"
#define RESET_FG "\033[0m"

#define RED_BG "\033[41m"
#define YELLOW_BG "\033[43m"
#define GREEN_BG "\033[42m"
#define BLUE_BG "\033[44m"
#define MAGENTA_BG "\033[45m"
#define CYAN_BG "\033[46m"
#define WHITE_BG "\033[47m"
#define RESET_BG "\033[49m"

#define PRINT_COLOR(color, func) \
	do { \
		printf(color); \
		func; \
		printf(RESET_FG); \
	} while (0)

#define PRINT_COLOR_BG(color, bg, func) \
	do { \
		printf(color); \
		printf(bg); \
		func; \
		printf(RESET_FG); \
		printf(RESET_BG); \
	} while (0)

#endif