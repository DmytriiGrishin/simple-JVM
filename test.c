#include "launcher.h"
#include "execute_engine.h"


int main (int argc, char const* const* argv){
	if(argc < 2) {fprintf(stderr, "Usage: vm file1 .. fileN\n"); return 2;}
	struct v_machine vm;

	struct class_header* class = malloc(sizeof(struct class_header)); 
	argv++;
	enum v_loader_status load_status = load_file(*argv, class, stderr);
	if (load_status == 	LOADER_IO_ERROR) return LAUNCH_FILE_ERROR;
	if (load_status == LOADER_FORMAT_ERROR || load_status == LOADER_VERSION_ERROR) return LAUNCH_FORMAT_ERROR;
	printf("LOADED\n");
	printf("magic: %X \nminor_version: %X \nmajor_version: %x \nconst pool count: %X\n %x\n%x\n%x\n%x\n", class->magic, class->minor_version, class->major_version, class->constant_pool_count, class->access_flags, class->this_class, class->super_class, class->interfaces_count);
	struct class_heap_t* class_heap = init_heap(class);
	printf("%s\n",class_heap->classes[0]->name);
	add_heap(class, class_heap);
	struct class_header* found  = get_by_name_heap("Test", class_heap);
	uint16_t main_index = find_method(class,"main");
	uint16_t code_index = find_code_attribute(class, main_index);

	printf("%d\n", main_index);
		printf("%d\n", code_index);
		printf("code: \nname index: %d\n", class->methods[main_index].attributes[code_index].attribute_name_index);
	struct code_attribute* code = malloc(sizeof(struct code_attribute));
	code->attribute_name_index = class->methods[main_index].attributes[code_index].attribute_name_index;
	code->attribute_length = class->methods[main_index].attributes[code_index].attribute_length;
	code->max_stack = class->methods[main_index].attributes[code_index].info[0]<<8 | class->methods[main_index].attributes[code_index].info[1];
	code->max_locals = class->methods[main_index].attributes[code_index].info[2]<<8 | class->methods[main_index].attributes[code_index].info[3];
	code->code_length = class->methods[main_index].attributes[code_index].info[4]<<24 | class->methods[main_index].attributes[code_index].info[5] << 16 | class->methods[main_index].attributes[code_index].info[6] << 8 | class->methods[main_index].attributes[code_index].info[7] ;
printf("length %d\n", code->code_length);
	code->code = class->methods[main_index].attributes[code_index].info +8;
	for (int i = 0; i < code->code_length; ++i){
		printf("%d\n", code->code[i]);
	}
	return LAUNCH_OK;
}



