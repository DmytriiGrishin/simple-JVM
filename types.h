#pragma once
#include <stdint.h>
union variable{
	uint8_t charValue;
	uint16_t shortValue;
	uint32_t intValue;
	float floatValue;
	uint64_t ptrValue;
};

struct frame{
	union variable* stack;
	struct class_header* class;
	uint64_t pc;
	uint8_t  stack_merge;
	union variable* local_vars;
};