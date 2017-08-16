#pragma once
#include <stdlib.h>
#include "class.h"
#include <string.h>

struct class_impl{
	struct class_header* class;
	char* name;
};

struct class_heap_t{
	struct class_impl** classes;
	uint64_t count;
};
struct class_heap_t* init_heap(struct class_header* first);

int add_heap(struct class_header* this, struct class_heap_t* heap);

struct class_header* get_by_name_heap();

char* get_class_name(struct class_header* from);