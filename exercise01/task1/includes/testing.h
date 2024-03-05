#ifndef TESTING_H
#define TESTING_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colorfull_print.h"
#include "memory_tracking.h"

#define PRINT_ERROR(error_print) PRINT_COLOR(RED_FG, error_print)
#define PRINT_WARNING(warning_print) PRINT_COLOR(YELLOW_FG, warning_print)
#define PRINT_SUCCESS(success_print) PRINT_COLOR(GREEN_FG, success_print)

extern int __tt__test_count__;
extern int __tt__test_failed__;

#define RUN_TEST(test) \
	do { \
		__tt__test_count__++; \
		test(); \
	} while (0)

#define RUN_TEST_WITH_SETUP_CLEANUP(test, setup, cleanup, injected_var) \
	do { \
		__tt__test_count__++; \
		injected_var var_##test = setup(); \
		test(var_##test); \
		cleanup(var_##test); \
	} while (0)

#define PRINT_TEST_RESULTS() \
	do { \
		printf("\n== Test Summary ==\n"); \
		printf(" Tests run:    %d\n", __tt__test_count__); \
		PRINT_SUCCESS(printf(" Tests passed: %d\n", __tt__test_count__ - __tt__test_failed__)); \
		PRINT_ERROR(printf(" Tests failed: %d\n", __tt__test_failed__)); \
		printf("==================\n"); \
	} while (0)

#define ASSERT(condition, error_msg, ...) \
	if (!(condition)) { \
		PRINT_ERROR(printf("On line %d: " error_msg "\n", __LINE__ __VA_OPT__(, ) __VA_ARGS__)); \
		__tt__test_failed__++; \
		return; \
	}

#define ASSERT_TRUE(condition) \
	ASSERT(condition, "Assertion failed: %s", condition ? "true" : "false")
#define ASSERT_FALSE(condition) \
	ASSERT(!(condition), "Assertion failed: %s", condition ? "false" : "true")
#define ASSERT_EQUALS(expected, actual) \
	ASSERT((expected) == (actual), "Assertion failed: %s == %s", #expected, #actual)
#define ASSERT_NOT_EQUALS(expected, actual) \
	ASSERT((expected) != (actual), "Assertion failed: %s != %s", #expected, #actual)
#define ASSERT_STRING_EQUALS(expected, actual) \
	ASSERT(strcmp((expected), (actual)) == 0, "Assertion failed: %s == %s", expected, actual)
#define ASSERT_STRING_NOT_EQUALS(expected, actual) \
	ASSERT(strcmp((expected), (actual)) != 0, "Assertion failed: %s != %s", expected, actual)
#define ASSERT_NULL(ptr) ASSERT((ptr) == NULL, "Assertion failed: %s == NULL", #ptr)
#define ASSERT_NOT_NULL(ptr) ASSERT((ptr) != NULL, "Assertion failed: %s != NULL", #ptr)
#define ASSERT_MEMORY_LEAK() ASSERT(check_memory_leak(), "Memory leak detected")
#define ASSERT_CLEAN_MEMORY_HANDLING() \
	ASSERT(check_clean_memory_handling(), "Memory not handled correctly")

#endif