#pragma once 
#include "types.h"
#include "class.h"
#include <string.h>
#include <stdlib.h>
#include "v_machine.h"

enum v_interpreter_status{
	INTER_OK=0,
	INTER_NO_WORD,
	INTER_NO_CLASS
};


uint16_t find_main_method(struct class_header* this);
struct v_machine* vm_init(struct class_heap_t* heap, struct class_header* class, struct v_machine* vm );
enum v_interpreter_status interpret(struct v_machine* vm);
uint8_t vm_push(struct v_stack_frame* frame, int64_t value);
int64_t vm_pop(struct v_stack_frame* frame);