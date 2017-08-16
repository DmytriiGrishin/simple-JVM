#pragma once
#include <stdint.h>

struct v_machine{
	struct v_stack_frame* stack;
	uint64_t stack_size;
	struct class_heap_t* class_heap;
};

struct v_stack_frame{
	uint8_t* stack;
	uint64_t* array;
	uint32_t code_length;
	uint8_t* code;
	struct class_header* current_class;
	uint64_t pc;
	struct v_stack_frame* prev_frame; 
};



