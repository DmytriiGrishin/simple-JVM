#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdint.h>
#include "class.h"
#include "class_heap.h" 
enum v_loader_status{
	LOADER_OK=0,
	LOADER_VERSION_ERROR,
	LOADER_FORMAT_ERROR,
	LOADER_IO_ERROR,
	LOADER_ARGUMENTS_ERROR
};
enum v_loader_status load_file(char const* name, struct class_header* class, FILE* err);

//enum v_loader_status load_files(char const* const* names, size_t count, struct v_prog* result, FILE* err);
