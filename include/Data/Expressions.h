#pragma once

#include <cstdint>

struct Expression {
	const signed char *eyeSdf;
	const char *name;

	Expression(const signed char *eyeSdf, const char *name) : eyeSdf(eyeSdf), name(name) {}
};

#define EXPRESSION_HAPPY 0
#define EXPRESSION_VWV 1
#define EXPRESSION_ANGRY 2
#define EXPRESSION_SPOOKED 3
#define EXPRESSION_BLUSH 4
#define EXPRESSION_DEAD 5

#define NUM_OF_EXPRESSIONS 6

extern Expression Expressions[];