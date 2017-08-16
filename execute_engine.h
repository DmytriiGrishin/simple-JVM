#pragma once 
#include "types.h"
#include "class.h"

uint16_t find_main_method(struct class_header* this);
struct v_machine* vm_init(struct class_heap_t* heap, struct class_header* class, struct v_machine* vm );
