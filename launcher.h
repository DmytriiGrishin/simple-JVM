#pragma once
#include <stdio.h>
#include "loader.h"
#include "v_machine.h"
#include "class_heap.h"
enum launch_status {
	LAUNCH_OK=0,
	LAUNCH_FILE_ERROR,
	LAUNCH_FORMAT_ERROR,
	LAUNCH_MAIN_ERROR
};