#include "types.h"
#include "class.h"
#include <string.h>
#include <stdlib.h>
#include "v_machine.h"





uint16_t find_method(struct class_header* this, char* name){
	uint16_t m_count = this->methods_count;
	uint16_t i;
	for(i=0; i<m_count; i++){
		uint16_t name_index = this->methods[i].name_index;
		if(!strcmp(name, (char*)this->constant_pool[name_index]->info)){
			return i;
		}
	}
} 


uint16_t find_code_attribute(struct class_header* this, uint16_t method_id){
	uint16_t attr_count = this->methods[method_id].attributes_count;
	uint16_t i;
	for(i=0; i<attr_count; i++){
		uint16_t attr_name_id = this->methods[method_id].attributes[i].attribute_name_index;
		if(!strcmp("Code", (char*)this->constant_pool[attr_name_id]->info)){
			return i;
		} 
	}
}
int vm_add_frame(struct v_machine* vm, struct v_stack_frame* frame){
	frame->prev_frame = (vm->stack);
	vm->stack = frame;
	vm->stack_size++;
	return 0;
}

struct v_stack_frame* vm_init_frame(struct class_header* class, char* method_name){
	struct v_stack_frame* ret = malloc(sizeof(struct v_stack_frame));
	ret->current_class = class;
	uint16_t main_index = find_method(class, method_name);
	uint16_t code_index = find_code_attribute(class, main_index);
	ret->stack = malloc((class->methods[main_index].attributes[code_index].info[0]<<8 | class->methods[main_index].attributes[code_index].info[1])*sizeof(uint8_t));
	ret->array = malloc(sizeof(uint8_t)* (class->methods[main_index].attributes[code_index].info[2]<<8 | class->methods[main_index].attributes[code_index].info[3]));
	ret->code_length = class->methods[main_index].attributes[code_index].info[4]<<24 | class->methods[main_index].attributes[code_index].info[5] << 16 | class->methods[main_index].attributes[code_index].info[6] << 8 | class->methods[main_index].attributes[code_index].info[7] ;
	ret->code = class->methods[main_index].attributes[code_index].info + 8;
	ret->pc =0;
	return ret;
}

struct v_machine* vm_init(struct class_heap_t* heap, struct class_header* class, struct v_machine* vm ){
	vm->class_heap = heap;
	vm->stack_size = 0;
	vm->stack = 0;
	vm_add_frame(vm, vm_init_frame(class, "main"));
	return vm;
}



