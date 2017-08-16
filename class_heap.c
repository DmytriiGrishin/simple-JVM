#include "class_heap.h"
char* get_class_name(struct class_header* from){
	uint16_t name_id = from->constant_pool[from->this_class]->info[0];
	name_id << 8; 
	name_id = from->constant_pool[from->this_class]->info[1];
	return (char*)from->constant_pool[name_id]->info;
}
struct class_heap_t* init_heap(struct class_header* first){
	struct class_heap_t* ret = malloc(sizeof(struct class_heap_t));
	ret->classes= malloc(sizeof(struct class_impl));
	ret->classes[0] = malloc(sizeof(struct class_impl));
	ret->classes[0]->class=first;
	ret->classes[0]->name = get_class_name(first);
	ret->count = 1;
	return ret;
}

int add_heap(struct class_header* this, struct class_heap_t* heap){
	int count = heap->count;
	heap->classes[count] = malloc(sizeof(struct class_impl));
	heap->classes[count]->class = this;
	heap->classes[count]->name = get_class_name(this);
	heap->count++;
	return 0; 	
}

struct class_header* get_by_name_heap(char* name, struct class_heap_t* heap){
	for(long long i =heap->count-1; i>0; i--){
		if (!strcmp(name, heap->classes[i]->name) ){
			return heap->classes[i]->class;
		} 
	}
	return NULL;
}



